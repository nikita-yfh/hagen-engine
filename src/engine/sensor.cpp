#include "sensor.hpp"
#include "main.hpp"
#include "utility.hpp"
#include "interface.hpp"
#include "lua.hpp"
namespace sensors{
vector<Sensor>sensors;
Sensor none;
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
	key=lua::get_scancode(k_id);
}
void Sensor::draw(){
	if(!enabled)return;
	GPU_BlitRect(textures["sensors.png"],&image,ren,&pos);
}
bool Sensor::update(int x,int y,uint8_t state){
	pactive=active;
	bool r=in_rect(x,y,pos);
	active=state && r;
	return r;
}
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
	return none;
}
Sensor &find(string key){
	return find(lua::get_scancode(key));
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
