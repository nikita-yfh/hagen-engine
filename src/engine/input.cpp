#include "input.hpp"
#include "main.hpp"
#include "utility.hpp"

SDL_Event e;
uint8_t prev_key[SDL_NUM_SCANCODES];
void copy_prev_key() {//для определения нажатий
	for(int q=0;q<SDL_NUM_SCANCODES;q++){
		prev_key[q]=key(q);
	}
}

short get_scancode(string k) {
	if(k=="up")		return SDL_SCANCODE_W;
	if(k=="down")	return SDL_SCANCODE_S;
	if(k=="left")	return SDL_SCANCODE_A;
	if(k=="right")	return SDL_SCANCODE_D;
	if(k=="jump")	return SDL_SCANCODE_SPACE;
	if(k=="action")	return SDL_SCANCODE_E;
	if(k=="1")		return SDL_SCANCODE_1;
	if(k=="2")		return SDL_SCANCODE_2;
	if(k=="3")		return SDL_SCANCODE_3;
	if(k=="4")		return SDL_SCANCODE_4;
	if(k=="5")		return SDL_SCANCODE_5;
	if(k=="6")		return SDL_SCANCODE_6;
	if(k=="7")		return SDL_SCANCODE_7;
	if(k=="8")		return SDL_SCANCODE_8;
	if(k=="9")		return SDL_SCANCODE_9;
	if(k=="0")		return SDL_SCANCODE_0;
	if(k=="esc")	return SDL_SCANCODE_ESCAPE;
	if(k=="console")return SDL_SCANCODE_GRAVE;
	if(k=="qsave")	return SDL_SCANCODE_F5;
	if(k=="qload")	return SDL_SCANCODE_F9;
	if(k=="back")	return SDL_SCANCODE_AC_BACK;
	return -1;
}
vector<Sensor>sensors;
Sensor::Sensor(){
	pos={0,0,0,0};
	image={0,0,0,0};
	enabled=false;
	active=false;
	pactive=false;
	key=-1;
}
Sensor::Sensor(XMLNode node){
	load_value(node,"position",pos);
	load_value(node,"image",image);
	enabled=node.getAttributei("enabled");
	string k_id=node.getAttribute("key");
	key=get_scancode(k_id);
}
void Sensor::draw(){
	if(!enabled)return;
	GPU_BlitRect(textures["sensors.png"],&image,ren,&pos);
}
bool Sensor::update(int x,int y,uint8_t state){
	pactive=active;
	bool r=in_rect(x,y,pos);
	if(state!=2)
		active=(state==1) && r;
	return active;
}
namespace sensor{
void load(){
	XMLNode Main=open_xml((prefix+"config/sensors.xml").c_str(),"sensors");
	int count=Main.getAttributei("count");
	for(int q=0;q<count;q++)
		sensors.push_back(Sensor(Main.getChildNode("sensor",q)));
}
Sensor &find(short key){
	for(auto &sensor : sensors)
		if(sensor.key==key)
			return sensor;
	static Sensor none;
	none=Sensor();
	return none;
}
Sensor &find(string key){
	return find(get_scancode(key));
}
void draw(){
	if(sensors.size())
		load_texture("sensors.png"); //общая текстура сенсорных кнопок
	for(auto &sensor : sensors)
		sensor.draw();
}
bool update(int x,int y,uint8_t state){
	for(auto &sensor : sensors)
		if(sensor.enabled && sensor.update(x,y,state))
			return 1;
	return 0;
}
bool get(short key){
	return find(key).active;
}
bool pget(short key){
	return find(key).active && !find(key).pactive;
}
};
float mouse_angle(){
	return mouse.angle;
}
float Mouse::g_angle() {
	if(interface.mainmenu.shown)return 0.0f;
	float px=SW/2-x;
	float py=SH/2-y;
	return get_angle(px,py);
}
bool Mouse::update() {
#ifdef ANDROID
	int mx=e.tfinger.x*SW;
	int my=e.tfinger.y*SH;
#else
	int mx=e.button.x;
	int my=e.button.y;
#endif
	if(g_state==Down)
		g_state=Press;
	else if(g_state==Down)
		g_state=None;
	if(g_state!=Press && g_state !=Up) {
		if(e.type==SDL_MOUSEBUTTONDOWN) {//Нажатие кнопки мыши
			g_state=Down;
			b=e.button.button;
		} else if(e.type==SDL_FINGERDOWN) {//нажатие пальцем
			g_state=Down;
			b=1;//при нажатии пальцем всегда эмуляция ЛКМ
		}
	} else if((e.type==SDL_MOUSEBUTTONUP || e.type==SDL_FINGERUP) && g_state !=None){//Поднятие пальца/кнопки мыши
		g_state=Up;
		sensor_press=0;
	}
	if(sensor::update(mx,my,g_state) && g_state==Down)//если нажал на какую-то сенсорную кнопку
		sensor_press=1;
	if(!sensor_press && g_state!=Up){//если не нажат сенсор
		state=g_state;
		if((e.type==SDL_MOUSEMOTION||e.type==SDL_FINGERMOTION) || g_state==Down){//Можно изменять координаты и угол только при движении/нажатии
			if(mx>=0&&mx<SW&&my>=0&&my<SH) {//если курсор/палец в пределах экрана
				x=mx;
				y=my;
				angle=g_angle();
			}
		}
	}
	//info_log(format("g:%d,l:%d,     angle:%g,    x:%d,y:%d, lua:%d",g_state,state,angle,x,y,lua::get_press_key("fire1")));
	return 0;
}
void Mouse::clear() {
	g_state=0;
	state=0;
	b=0;
}

bool key(short code){
	return SDL_GetKeyboardState(0)[code] || sensor::get(code);
}
bool pkey(short code){
	return (!prev_key[code] && SDL_GetKeyboardState(0)[code]) || sensor::pget(code);
}

float get_angle(float x,float y){
	float a;
	if(x>=0)
		a= M_PI+atan(y/x);
	else if(y>0)
		a= 2*M_PI+atan(y/x);
	else
		a= atan(y/x);
	return a;
}
