#include "entity.hpp"
#include "level.hpp"
#include "physic.hpp"
using namespace std;
Entity::Entity() {};
Entity::Entity(string _type,string _id,float xp,float yp) {
	type=_type;
	id=_id;
	XMLNode xml=XMLNode::openFileHelper(("entities/"+type+".xml").c_str(),"physics");
	{
		//bodies
		XMLNode bds=xml.getChildNode("bodies");
		int bodies_count=stoi(bds.getAttribute("count"));
		bodies.resize(bodies_count);
		for(int q=0; q<bodies_count; q++) {
			bodies[q]=read_body(bds.getChildNode("body",q),{xp,yp});
		}
	}
	{
		//joints
		XMLNode js=xml.getChildNode("joints");
		int joints_count=stoi(js.getAttribute("count"));
		joints.resize(joints_count);
		for(int q=0; q<joints_count; q++) {
			joints[q]=0;
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")!="GearJoint")
				joints[q]=read_joint(ch,{xp,yp},this);
		}
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")=="GearJoint")
				joints[q]=read_joint(ch,{xp,yp},this);
		}
	}
}
Entity::~Entity(){}

b2Body *Entity::get_body(string id) {
	for(int q=0; q<bodies.size(); q++)
		if(id==B_DATA(bodies[q],id))return bodies[q];
	throw string("\""+id+ "\" is not a body");
}

b2Joint *Entity::get_joint(string id) {
	for(int q=0; q<joints.size(); q++)
		if(id==J_DATA(joints[q],id))return joints[q];
	throw string("\""+id+ "\" is not a joint");
}
