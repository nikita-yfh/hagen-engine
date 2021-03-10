#include "entity.hpp"
#include "level.hpp"
#include "physic.hpp"
using namespace std;
Entity::Entity() {};
Entity::Entity(string _type,float xp,float yp) {
	type=_type;
	XMLNode xml=XMLNode::openFileHelper(("entities/"+type+".xml").c_str(),"physics");
	{
		//bodies
		XMLNode bds=xml.getChildNode("bodies");
		int bodies_count=stoi(bds.getAttribute("count"));
		bodies.clear();
		for(int q=0; q<bodies_count; q++) {
			string str;
			b2Body *b=read_body(bds.getChildNode("body",q),str,{xp,yp});
			bodies[str]=b;
		}
	}
	{
		//joints
		XMLNode js=xml.getChildNode("joints");
		int joints_count=stoi(js.getAttribute("count"));
		joints.clear();
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")!="GearJoint"){
				string str;
				b2Joint *j=read_joint(ch,str,{xp,yp},this);
				joints[str]=j;
			}
		}
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")=="GearJoint"){
				string str;
				b2Joint *j=read_joint(ch,str,{xp,yp},this);
				joints[str]=j;
			}
		}
	}
}
Entity::~Entity(){}

b2Body *Entity::get_body(string id) {
	return bodies[id];
}

b2Joint *Entity::get_joint(string id) {
	return joints[id];
}
