#include "input.hpp"
#include "utility.hpp"
#include "interface.hpp"
#include "camera.hpp"

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
	if(k=="action")	return SDL_SCANCODE_E;
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

static bool down(){
	return e.type==SDL_MOUSEBUTTONDOWN || e.type==SDL_FINGERDOWN;
}
static bool up(){
	return e.type==SDL_MOUSEBUTTONUP || e.type==SDL_FINGERUP;
}
static bool motion(){
	return e.type==SDL_MOUSEMOTION || e.type==SDL_FINGERMOTION;
}
#ifdef TOUCH
static uint16_t fid(){
	return e.tfinger.fingerId;
}
static int get_num_fingers(){
	return SDL_GetNumTouchFingers(e.tfinger.touchId);
}
static SDL_Finger* get_finger(int n){
	return SDL_GetTouchFinger(e.tfinger.touchId, n);
}
static bool in_rect(GPU_Rect rect){
	int numfingers = get_num_fingers();
	for (int i = 0; i < numfingers; i++){
		SDL_Finger *finger =get_finger(i);
		if (finger->id == e.tfinger.fingerId)
			if(in_rect(finger->x*SW,finger->y*SH,rect))
				return 1;
	}
	return 0;
}
vector<Sensor>sensors;
Sensor::Sensor(){
	pos={0,0,0,0};
	image={0,0,0,0};
	enabled=false;
	active=false;
	pactive=false;
	key=-1;
	id=-1;
}
Sensor::Sensor(XMLNode node){
	load_value(node,"position",pos);
	load_value(node,"image",image);
	enabled=node.getAttributei("enabled");
	string k_id=node.getAttribute("key");
	key=get_scancode(k_id);
	id=-1;
	active=false;
	pactive=false;
}
void Sensor::draw(){
	if(!enabled)return;
	GPU_BlitRect(textures["sensors.png"],&image,ren,&pos);
}
bool Sensor::update(){
	pactive=active;
	bool r=in_rect(pos);
	if(up() && id==fid()){
		active=0;
		id=-1;
	}else if(down() && r){
		id=fid();
		active=1;
	}
	return id==fid();
}
namespace sensor{
static bool enabled=false;
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
	if(enabled){
		if(sensors.size())
			load_texture("sensors.png"); //общая текстура сенсорных кнопок
		for(auto &sensor : sensors)
			sensor.draw();
	}
}
bool update(){
	bool ok=0;
	enabled=!interface.mainmenu.shown;
	if(enabled){
		for(auto &sensor : sensors)
			if(sensor.enabled && sensor.update())
				ok=1;
	}
	return ok;
}
bool get(short key){
	return find(key).active;
}
bool pget(short key){
	return find(key).active && !find(key).pactive;
}
};
#endif
float mouse_angle(){
	return mouse.angle;
}
float Mouse::g_angle() {
	if(interface.mainmenu.shown)return 0.0f;
	info_log(format("cx:%g cy:%g x:%g y:%g",ix,iy,cx,cy));
	float px=SW/2+(ix-cx)*zoom-x;
	float py=SH/2+(iy-cy)*zoom-y;
	return get_angle(px,py);
}
void Mouse::update() {
	if(state==Down)
		state=Press;
	else if(state==Up)
		state=None;
#ifdef TOUCH
	if(!sensor::update() && state!=Press && state !=Up && down()) {//Нажатие пальцем
		b=1;
		state=Down;
		id=fid();
	} else if(up() && state !=None && id==fid()){//Поднятие пальца
		state=Up;
		id=-1;
	}

	int numfingers = get_num_fingers();
	for (int i = 0; i < numfingers; i++){
		SDL_Finger *f =get_finger(i);
		if (f->id == id){
			x=f->x*SW;
			y=f->y*SH;
			angle=g_angle();
		}
	}
#else
	if(state!=Press && state !=Up && down()) {//Нажатие кнопкой мыши
		b=e.button.button;
		state=Down;
	} else if(up() && state !=None)//Поднятие кнопки мыши
		state=Up;
	int mx=0;int my=0;
	SDL_GetMouseState(&mx,&my);
	if(mx>=0&&mx<SW&&my>=0&&my<SH) {//если курсор/палец в пределах экрана
		x=mx;
		y=my;
		angle=g_angle();
	}
#endif
}
void Mouse::clear() {
	state=0;
	b=0;
#ifdef TOUCH
	id=-1;
#endif
}
bool key(short code){
	return SDL_GetKeyboardState(0)[code]
#ifdef TOUCH
	|| sensor::get(code)
#endif
;
}
bool pkey(short code){
	return (!prev_key[code] && SDL_GetKeyboardState(0)[code])
#ifdef TOUCH
	|| sensor::pget(code)
#endif
;
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
