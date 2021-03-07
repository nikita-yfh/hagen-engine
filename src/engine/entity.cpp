#include "entity.hpp"
#include "level.hpp"
using namespace std;
Entity::Entity() {};
Entity::Entity(string _type,string _id,float xp,float yp) {
	/*for(Entity &en : entities){
		if(en.type==type){
			*this=en;
			b2Vec2 d={xp-en.p.x,yp-en.p.y};
			for(int q=0;q<bodies.size();q++)
				bodies[q]->m_xf.p+=d;
			return;
		}
	}*/
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
				joints[q]=read_joint(ch);
		}
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")=="GearJoint")
				joints[q]=read_joint(ch);
		}
	}
}
Entity::~Entity(){}
