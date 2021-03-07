#include "level.hpp"
#include "utils.hpp"
using namespace std;
Level level;
bool Level::save_file(std::string path,bool all) {
	XMLNode Main=XMLNode::createXMLTopNode("xml",1);
	Main.addAttribute("version","1.0");
	Main.addAttribute("encoding","UTF-8");
	{
		//level
		XMLNode lvl;
		if(all) {
			lvl=Main.addChild("level");
			lvl.addAttribute("w",w);
			lvl.addAttribute("h",h);
			{
				//background
				XMLNode bgr=lvl.addChild("background");
				bgr.addAttribute("img",background.c_str());
			}
		} else {
			lvl=Main.addChild("physics");
		}
		{
			//bodies
			XMLNode bds=lvl.addChild("bodies");
			bds.addAttribute("count",bodies.size());
			for(int q=0; q<bodies.size(); q++) { //body
				bodies[q]->save(bds,1);
			}
		}
		if(all){
			//entities
			XMLNode ens=lvl.addChild("entities");
			ens.addAttribute("count",entities.size());
			for(int q=0; q<entities.size(); q++) { //body
				XMLNode en=ens.addChild("entity");
				XMLNode pos=en.addChild("position");
				pos.addAttribute("x",entities[q]->x);
				pos.addAttribute("y",entities[q]->y);
				en.addAttribute("id",entities[q]->id);
				en.addAttribute("type",entities[q]->type);
			}
		}
		save_joints(lvl);
	}
	if(Main.writeToFile(path.c_str(),"UTF-8",1))return 1;
	return 0;
}
bool Level::open_file(string path) {
	bool all=1;
	XMLNode lvl=XMLNode::openFileHelper(path.c_str(),"level",0);
	if(lvl.isEmpty()) {
		lvl=XMLNode::openFileHelper(path.c_str(),"physics");
		all=0;
	}
	if(all) {
		w=stoi(lvl.getAttribute("w"));
		h=stoi(lvl.getAttribute("h"));
		{
			//backgroung
			XMLNode bgr=lvl.getChildNode("background");
			background=bgr.getAttribute("img");
		}
	}
	{
		//bodies
		XMLNode bds=lvl.getChildNode("bodies");
		for(int q=0; q<bodies.size(); q++) {
			delete bodies[q];
		}
		bodies.resize(stoi(bds.getAttribute("count")));
		for(int q=0; q<bodies.size(); q++) {
			XMLNode bd=bds.getChildNode("body",q);
			bodies[q]=new Body;
			bodies[q]->load(bd,1);
		}
	}
	if(all){
		//entities
		XMLNode ens=lvl.getChildNode("entities");
		for(int q=0; q<entities.size(); q++) {
			delete entities[q];
		}
		entities.resize(stoi(ens.getAttribute("count")));
		for(int q=0; q<entities.size(); q++) {
			XMLNode en=ens.getChildNode("entity",q);
			XMLNode pos=en.getChildNode("position");
			entities[q]=new Entity;
			entities[q]->x=stof(pos.getAttribute("x"));
			entities[q]->y=stof(pos.getAttribute("y"));
			entities[q]->id=en.getAttribute("id");
			entities[q]->type=en.getAttribute("type");
		}
	}
	load_joints(lvl);
	return 0;
}
void Level::save_joints(XMLNode &lvl) {
	XMLNode jall=lvl.addChild("joints");
	jall.addAttribute("count",joints.size());
	for(Joint *j : joints) {
		XMLNode jnode=jall.addChild("joint");
		jnode.addAttribute("type",j->name());
		jnode.addAttribute("id",j->id);
		{
			XMLNode connected=jnode.addChild("connected");
			connected.addAttribute("collide",j->collide);
			connected.addAttribute("id1",j->id1.c_str());
			connected.addAttribute("id2",j->id2.c_str());
		}
		XMLNode pos=jnode.addChild("position");
		XMLNode phs=jnode.addChild("physic");
		if(j->name()=="WeldJoint") {
			WeldJoint *joint=TYPE(WeldJoint*,j);
			pos.addAttribute("x",joint->x);
			pos.addAttribute("y",joint->y);
			phs.addAttribute("stiffness",joint->stiffness);
			phs.addAttribute("damping",joint->damping);
		} else if(j->name()=="RevoluteJoint") {
			RevoluteJoint *joint=TYPE(RevoluteJoint*,j);
			pos.addAttribute("x",joint->x);
			pos.addAttribute("y",joint->y);
			phs.addAttribute("limit",joint->limit);
			if(joint->limit) {
				phs.addAttribute("lower",joint->lower);
				phs.addAttribute("upper",joint->upper);
			}
			phs.addAttribute("motor",joint->motor);
			if(joint->motor) {
				phs.addAttribute("speed",joint->speed);
				phs.addAttribute("max_torque",joint->torque);
			}
		} else if(j->name()=="DistanceJoint") {
			DistanceJoint *joint=TYPE(DistanceJoint*,j);
			pos.addAttribute("x1",joint->x1);
			pos.addAttribute("y1",joint->y1);
			pos.addAttribute("x2",joint->x2);
			pos.addAttribute("y2",joint->y2);
			pos.addAttribute("min",joint->min);
			pos.addAttribute("max",joint->max);
			phs.addAttribute("stiffness",joint->stiffness);
			phs.addAttribute("damping",joint->damping);
		} else if(j->name()=="PulleyJoint") {
			PulleyJoint *joint=TYPE(PulleyJoint*,j);
			pos.addAttribute("x1",joint->x1);
			pos.addAttribute("y1",joint->y1);
			pos.addAttribute("x2",joint->x2);
			pos.addAttribute("y2",joint->y2);
			pos.addAttribute("x3",joint->x3);
			pos.addAttribute("y3",joint->y3);
			pos.addAttribute("x4",joint->x4);
			pos.addAttribute("y4",joint->y4);
			phs.addAttribute("ratio",joint->ratio);
		} else if(j->name()=="PrismaticJoint") {
			PrismaticJoint *joint=TYPE(PrismaticJoint*,j);
			pos.addAttribute("x",joint->x);
			pos.addAttribute("y",joint->y);
			pos.addAttribute("angle",joint->angle);
			phs.addAttribute("limit",joint->limit);
			if(joint->limit) {
				phs.addAttribute("lower",joint->lower);
				phs.addAttribute("upper",joint->upper);
			}
			phs.addAttribute("motor",joint->motor);
			if(joint->motor) {
				phs.addAttribute("speed",joint->speed);
				phs.addAttribute("max_force",joint->force);
			}
		} else if(j->name()=="GearJoint") {
			GearJoint *joint=TYPE(GearJoint*,j);
			phs.addAttribute("ratio",joint->ratio);
		}
	}
}
void Level::load_joints(XMLNode &lvl) {
	XMLNode jall=lvl.getChildNode("joints");
	int joints_count=stoi(jall.getAttribute("count"));
	for(int q=0; q<joints.size(); q++)
		delete joints[q];
	joints.resize(joints_count);
	for(int q=0; q<joints_count; q++) {
		XMLNode node=jall.getChildNode("joint",q);
		XMLNode pos=node.getChildNode("position");
		XMLNode phs=node.getChildNode("physic");
		XMLNode con=node.getChildNode("connected");
		string type=node.getAttribute("type");
		if(type=="WeldJoint") {
			WeldJoint *j=new WeldJoint;
			j->x=to_fl(pos.getAttribute("x"));
			j->y=to_fl(pos.getAttribute("y"));
			j->stiffness=to_fl(phs.getAttribute("stiffness"));
			j->damping=to_fl(phs.getAttribute("damping"));
			joints[q]=j;
		} else if(type=="RevoluteJoint") {
			RevoluteJoint *j=new RevoluteJoint;
			j->x=to_fl(pos.getAttribute("x"));
			j->y=to_fl(pos.getAttribute("y"));
			j->limit=stoi(phs.getAttribute("limit"));
			j->motor=stoi(phs.getAttribute("motor"));
			if(j->limit) {
				j->lower=to_fl(phs.getAttribute("lower"));
				j->upper=to_fl(phs.getAttribute("upper"));
			}
			if(j->motor) {
				j->speed=to_fl(phs.getAttribute("speed"));
				j->torque=to_fl(phs.getAttribute("max_torque"));
			}
			joints[q]=j;
		} else if(type=="GearJoint") {
			GearJoint *j=new GearJoint;
			j->ratio=to_fl(phs.getAttribute("ratio"));
			joints[q]=j;
		} else if(type=="PrismaticJoint") {
			PrismaticJoint *j=new PrismaticJoint;
			j->x=to_fl(pos.getAttribute("x"));
			j->y=to_fl(pos.getAttribute("y"));
			j->angle=to_fl(pos.getAttribute("angle"));
			j->limit=stoi(phs.getAttribute("limit"));
			j->motor=stoi(phs.getAttribute("motor"));
			if(j->limit) {
				j->lower=to_fl(phs.getAttribute("lower"));
				j->upper=to_fl(phs.getAttribute("upper"));
			}
			if(j->motor) {
				j->speed=to_fl(phs.getAttribute("speed"));
				j->force=to_fl(phs.getAttribute("max_force"));
			}
			joints[q]=j;
		} else if(type=="DistanceJoint") {
			DistanceJoint *j=new DistanceJoint;
			j->x1=to_fl(pos.getAttribute("x1"));
			j->y1=to_fl(pos.getAttribute("y1"));
			j->x2=to_fl(pos.getAttribute("x2"));
			j->y2=to_fl(pos.getAttribute("y2"));
			j->min=to_fl(pos.getAttribute("min"));
			j->max=to_fl(pos.getAttribute("max"));
			j->stiffness=to_fl(phs.getAttribute("stiffness"));
			j->damping=to_fl(phs.getAttribute("damping"));
			joints[q]=j;
		} else if(type=="PulleyJoint") {
			PulleyJoint *j=new PulleyJoint;
			j->x1=to_fl(pos.getAttribute("x1"));
			j->y1=to_fl(pos.getAttribute("y1"));
			j->x2=to_fl(pos.getAttribute("x2"));
			j->y2=to_fl(pos.getAttribute("y2"));
			j->x3=to_fl(pos.getAttribute("x3"));
			j->y3=to_fl(pos.getAttribute("y3"));
			j->x4=to_fl(pos.getAttribute("x4"));
			j->y4=to_fl(pos.getAttribute("y4"));
			j->ratio=to_fl(phs.getAttribute("ratio"));
			joints[q]=j;
		}
		joints[q]->id1=con.getAttribute("id1");
		joints[q]->id2=con.getAttribute("id2");
		joints[q]->collide=stoi(con.getAttribute("collide"));
		joints[q]->id=node.getAttribute("id");
	}
}
