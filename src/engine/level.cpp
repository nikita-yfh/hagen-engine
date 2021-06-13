#include "level.hpp"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "physic.hpp"
#include "utility.hpp"
#include "lua.hpp"
#include "sdl.hpp"
#include "triangulate.hpp"
#include "main.hpp"
#include "camera.hpp"
#include "effect.hpp"
#include "text.hpp"
using namespace std;
unsigned short int levelw=20;
unsigned short int levelh=20;
map<string,b2Body*>bodies;
map<string,b2Joint*>joints;
map<string,Entity*>entities;
string background;
string levelname;
b2World *world=0;
void save_body_state(XMLNode bd,b2Body *body) {
	bd.addAttribute("id",body->GetID());
	bd.addAttribute("script",body->GetUserData()->script);
	bd.addAttribute("created",body->GetUserData()->created);
	XMLNode val=bd.addChild("userdata");
	lua::save_luaref(val,*body->m_userData->lua_userdata);
	save_value(bd,"transform",body->m_xf);
	save_value(bd,"force",body->m_force);
	save_value(bd,"torque",body->m_torque);
	save_value(bd,"ldamping",body->m_linearDamping);
	save_value(bd,"adamping",body->m_angularDamping);
	save_value(bd,"lvelocity",body->m_linearVelocity);
	save_value(bd,"avelocity",body->m_angularVelocity);
	save_value(bd,"sleep_time",body->m_sleepTime);
	save_value(bd,"sweep",body->m_sweep);
}
void load_body_state(XMLNode bd,b2Body *body) {
	XMLNode val=bd.getChildNode("userdata");
	if(body->m_userData->lua_userdata)
		delete body->m_userData->lua_userdata;
	body->m_userData->lua_userdata=new luabridge::LuaRef(lua::load_luaref(val));
	load_value(bd,"transform",body->m_xf);
	load_value(bd,"force",body->m_force);
	load_value(bd,"torque",body->m_torque);
	load_value(bd,"ldamping",body->m_linearDamping);
	load_value(bd,"adamping",body->m_angularDamping);
	load_value(bd,"lvelocity",body->m_linearVelocity);
	load_value(bd,"avelocity",body->m_angularVelocity);
	load_value(bd,"sleep_time",body->m_sleepTime);
	load_value(bd,"sweep",body->m_sweep);
}
void save_bodies_state(XMLNode bds,map<string,b2Body*>&bodies) {
	bds.addAttribute("count",bodies.size());
	for(auto &body : bodies)
		save_body_state(bds.addChild("body"),body.second);
}
void load_bodies_state(XMLNode bds,map<string,b2Body*>&bodies) {
	string s=bds.getAttribute("count");
	vector<string>loaded;
	int count=bds.getAttributei("count");
	for(int q=0; q<count; q++) {
		XMLNode bd=bds.getChildNode("body",q);
		string id=bd.getAttribute("id");
		string script=bd.getAttribute("script");
		bool created=bd.getAttributei("created");
		if(created)
			create_body(script,id,0,0);
		loaded.push_back(id);
		load_body_state(bd,bodies[id]);
	}
	bool erased=1;
	while(erased) {
		for(auto &body : bodies) {
			bool ok=0;
			for(auto &l : loaded)
				if(body.second->GetID()==l)
					ok=1;
			if(!ok) {
				destroy_body(body.second);
				erased=1;
				break;
			}
		}
		erased=0;
	}
}
void save_entities_state(XMLNode bds,map<string,Entity*>&entities) {
	bds.addAttribute("count",entities.size());
	for(auto &entity : entities) {
		XMLNode en=bds.addChild("entity");
		Entity *ent=entity.second;
		en.addAttribute("id",ent->id);
		en.addAttribute("type",ent->type);
		en.addAttribute("created",ent->created);
		XMLNode weapon=en.addChild("weapon");
		weapon.addAttribute("name",ent->weapon.name);
		weapon.addAttribute("bullet1",ent->weapon.bullet1);
		weapon.addAttribute("bullet2",ent->weapon.bullet2);
		weapon.addAttribute("angle",ent->weapon.angle);
		weapon.addAttribute("dx",ent->weapon.dx);
		weapon.addAttribute("dy",ent->weapon.dy);
		weapon.addAttribute("point_x",ent->weapon.point_x);
		weapon.addAttribute("point_y",ent->weapon.point_y);
		weapon.addAttribute("texture",ent->weapon.texture);
		XMLNode health=en.addChild("health");
		health.addAttribute("value",ent->health);
		health.addAttribute("max",ent->max_health);
		XMLNode val=en.addChild("userdata");
		lua::save_luaref(val,*ent->lua_userdata);
		save_bodies_state(en.addChild("bodies"),ent->bodies);
	}
}
void load_entities_state(XMLNode ens,map<string,Entity*>&entities) {
	int count=ens.getAttributei("count");
	vector<string>loaded;
	for(int q=0; q<count; q++) {
		XMLNode en=ens.getChildNode("entity",q);
		string id=en.getAttribute("id");
		string type=en.getAttribute("type");
		bool created=en.getAttributei("created");
		if(created)
			create_entity(type,id,0,0);
		loaded.push_back(id);
		Entity *entity=entities[id];
		XMLNode weapon=en.getChildNode("weapon");
		entity->weapon.name=weapon.getAttribute("name");
		entity->set_weapon(entity->weapon.name);
		entity->weapon.angle=weapon.getAttributef("angle");
		entity->weapon.bullet1=weapon.getAttribute("bullet1");
		entity->weapon.bullet2=weapon.getAttribute("bullet2");
		entity->weapon.dx=weapon.getAttributef("dx");
		entity->weapon.dy=weapon.getAttributef("dy");
		entity->weapon.point_x=weapon.getAttributef("point_x");
		entity->weapon.point_y=weapon.getAttributef("point_y");
		entity->weapon.texture=weapon.getAttribute("texture");
		XMLNode health=en.getChildNode("health");
		entity->health=health.getAttributef("value");
		entity->max_health=health.getAttributef("max");
		if(entity->lua_userdata)
			delete entity->lua_userdata;
		entity->lua_userdata=new luabridge::LuaRef(lua::load_luaref(en.getChildNode("userdata")));
		load_bodies_state(en.getChildNode("bodies"),entity->bodies);
	}
	bool erased=1;
	while(erased) {
		for(auto &entity : entities) {
			bool ok=0;
			for(auto &l : loaded)
				if(entity.second->id==l)
					ok=1;
			if(!ok) {
				destroy_entity(entity.second);
				erased=1;
				break;
			}
		}
		erased=0;
	}
}
void save_values_state(XMLNode data) {
	int count=0;
	for(string &str : lua::loaded) {
		if(lua::is_filled(luabridge::getGlobal(lua::L,str.c_str()))) {
			XMLNode v=data.addChild("value");
			v.addAttribute("name",str);
			lua::save_luaref(v,luabridge::getGlobal(lua::L,str.c_str()));
			count++;
		}
	}
	data.addAttribute("count",count);
}
void load_values_state(XMLNode data) {
	int count=data.getAttributei("count");
	for(int q=0; q<count; q++) {
		XMLNode bd=data.getChildNode("value",q);
		string str=bd.getAttribute("name");
		luabridge::getGlobal(lua::L,str.c_str())=lua::load_luaref(bd);
	}
}
void save_bullets_state(XMLNode bls) {
	int count=0;
	for(auto &b : bullets) {
		if(b.first!="") {
			XMLNode bl=bls.addChild("bullet");
			bl.addAttribute("name",b.first);
			bl.addAttribute("count",b.second.count);
			bl.addAttribute("max",b.second.max);
			count++;
		}
	}
	bls.addAttribute("count",count);
}
void load_bullets_state(XMLNode bls) {
	int count=bls.getAttributei("count");
	for(int q=0; q<count; q++) {
		XMLNode bl=bls.getChildNode("bullet",q);
		string id=bl.getAttribute("name");
		bullets[id].count=bl.getAttributei("count");
		bullets[id].max=bl.getAttributei("max");
	}
}
void save_world_state(string name) {
	XMLNode lvl=XMLNode::createXMLTopNode("level");
	lvl.addAttribute("name",levelname);
	{
		XMLNode time=lvl.addChild("time");
		time.addAttribute("value",lua::get_time());
		time.addAttribute("scale",lua::time_scale);
	}
	{
		XMLNode phs=lvl.addChild("physic");
		phs.addAttribute("velocity_iterations",velocity_iterations);
		phs.addAttribute("position_iterations",position_iterations);
	}
	save_value(lvl,"gravity",world->m_gravity);
	{
		XMLNode cam=lvl.addChild("camera");
		cam.addAttribute("x",cx);
		cam.addAttribute("y",cy);
		cam.addAttribute("zoom",zoom);
		cam.addAttribute("locked",camera_locked);
	}
	save_bodies_state(lvl.addChild("bodies"),bodies);
	save_entities_state(lvl.addChild("entities"),entities);
	save_values_state(lvl.addChild("values"));
	save_bullets_state(lvl.addChild("bullets"));
	lvl.writeToFile((saves+name+".xml").c_str());
	info_log("Saved to "+saves+name+".xml");
}
void load_world_state(string name) {
	XMLNode lvl=open_xml((saves+name+".xml").c_str(),"level");
	load_level(lvl.getAttribute("name"));
	{
		XMLNode time=lvl.getChildNode("time");
		lua::game_time=time.getAttributef("value");
		lua::time_scale=time.getAttributef("scale");
		lua::prev_time=SDL_GetTicks();
	}
	{
		XMLNode phs=lvl.getChildNode("physic");
		velocity_iterations=phs.getAttributei("velocity_iterations");
		position_iterations=phs.getAttributei("position_iterations");
	}
	load_value(lvl,"gravity",world->m_gravity);
	{
		XMLNode cam=lvl.getChildNode("camera");
		cx=cam.getAttributef("x");
		cy=cam.getAttributef("y");
		zoom=cam.getAttributef("zoom");
		camera_locked=cam.getAttribute("locked");
	}
	load_bodies_state(lvl.getChildNode("bodies"),bodies);
	load_entities_state(lvl.getChildNode("entities"),entities);
	load_values_state(lvl.getChildNode("values"));
	load_bullets_state(lvl.getChildNode("bullets"));
	info_log("Loaded from "+saves+name+".xml");
}
b2Body* read_body(XMLNode bd,b2Vec2 delta,bool temp) {
	int shapes_count=bd.getAttributei("shapes");
	if(shapes_count==0)
		throw string("Body \""+(string)bd.getAttribute("id")+"\" is empty");
	b2BodyDef def;
	b2Body *body;
	def.userData=new b2BodyData;
	if(!temp) {
		def.position=b2Vec2(bd.getAttributef("x"),
							bd.getAttributef("y"))+delta;
	} else def.position=delta;
	{
		XMLNode phs=bd.getChildNode("physic");
		def.fixedRotation=phs.getAttributei("fixed_rotation");
		def.bullet=phs.getAttributei("bullet");
		def.gravityScale=phs.getAttributef("gravity_scale");
	}
	{
		string str=bd.getAttribute("type");
		if(str=="static")           def.type=b2BodyType::b2_staticBody;
		else if(str=="dynamic")     def.type=b2BodyType::b2_dynamicBody;
		else if(str=="kinematic")   def.type=b2BodyType::b2_kinematicBody;
	}
	BD_DATA(def,script)=bd.getAttribute("script");
	body=world->CreateBody(&def);
	for(int j=0; j<shapes_count; j++) {
		XMLNode sh=bd.getChildNode("shape",j);
		b2FixtureDef fix;
		fix.userData=new b2FixtureData;
		FD_DATA(fix,texture)	=sh.getAttribute("texture");
		FD_DATA(fix,expand)		=sh.getAttributei("expand");
		FD_DATA(fix,id)			=sh.getAttribute("id");
		{
			//pos
			string str=sh.getAttribute("pos");
			if(str=="background")			FD_DATA(fix,pos)=TBGR;
			else if(str=="b_physic")		FD_DATA(fix,pos)=TPHS1;
			else if(str=="physic")			FD_DATA(fix,pos)=TPHS2;
			else if(str=="f_physic")		FD_DATA(fix,pos)=TPHS3;
			else if(str=="foreground")		FD_DATA(fix,pos)=TFGR;
			else if(str=="liquid")			FD_DATA(fix,pos)=TLIQUID;
			else if(str=="none")			FD_DATA(fix,pos)=TNONE;
			fix.isSensor=(FD_DATA(fix,pos)==TBGR||FD_DATA(fix,pos)>=TFGR);
			if(FD_DATA(fix,pos)==TLIQUID)fix.isSensor=2;
		}
		{
			//physic
			XMLNode phs=sh.getChildNode("physic");
			fix.density=    phs.getAttributef("density");
			fix.friction=   phs.getAttributef("friction");
			fix.restitution=phs.getAttributef("restitution");
		}
		{
			//collision
			XMLNode collision=sh.getChildNode("collision");
			fix.filter.categoryBits=    pow(2,collision.getAttributei("category"));
			fix.filter.maskBits=    	collision.getAttributei("mask");

		}
		{
			//position
			XMLNode pos=sh.getChildNode("position");
			string str=sh.getAttribute("type");
			if(str=="Square") {
				b2PolygonShape shape;
				float r=pos.getAttributef("r");
				shape.SetAsBox(r,r,b2Vec2(
								   pos.getAttributef("x"),
								   pos.getAttributef("y")),0);
				fix.shape=&shape;
				FD_DATA(fix,type)=SQUARE;
				body->CreateFixture(&fix);
			} else if(str=="Circle") {
				b2CircleShape shape;
				shape.m_radius=pos.getAttributef("r");
				shape.m_p.Set( pos.getAttributef("x"),
							   pos.getAttributef("y"));
				fix.shape=&shape;
				FD_DATA(fix,type)=CIRCLE;
				body->CreateFixture(&fix);
			} else if(str=="Rect") {
				b2PolygonShape shape;
				float xp1=pos.getAttributef("x1");
				float yp1=pos.getAttributef("y1");
				float xp2=pos.getAttributef("x2");
				float yp2=pos.getAttributef("y2");
				shape.SetAsBox(_abs(xp2-xp1)/2,_abs(yp2-yp1)/2,b2Vec2((xp1+xp2)/2,(yp1+yp2)/2),0);
				fix.shape=&shape;
				FD_DATA(fix,type)=RECT;
				body->CreateFixture(&fix);
			} else if(str=="Line") {
				b2EdgeShape shape;
				float x1=pos.getAttributef("x1");
				float y1=pos.getAttributef("y1");
				float x2=pos.getAttributef("x2");
				float y2=pos.getAttributef("y2");
				shape.SetTwoSided(b2Vec2(x1,y1),b2Vec2(x2,y2));
				fix.shape=&shape;
				FD_DATA(fix,type)=LINE;
				body->CreateFixture(&fix);
			} else if(str=="Polygon") {
				int count=pos.getAttributei("point_count");
				FD_DATA(fix,type)=POLYGON;
				Vector2dVector vec(count),result;
				b2Vec2 *big=new b2Vec2[count];

				for(int e=0; e<count; e++) {
					XMLNode point=pos.getChildNode("point",e);
					big[e].x=vec[e].x=point.getAttributef("x");
					big[e].y=vec[e].y=point.getAttributef("y");
				}

				Triangulate::Process(vec,result);
				for(int q=0; q<result.size(); q+=3) {
					b2FixtureDef fix2=fix;
					b2PolygonShape shape;
					shape.big_polygon=big;
					shape.b_count=count;
					b2Vec2 v[3];
					for(int e=0; e<3; e++)
						v[e]=result[q+e];
					shape.Set(v,3);
					fix2.shape=&shape;
					body->CreateFixture(&fix2);
				}
			} else if(str=="Cover") {
				int count=pos.getAttributei("point_count");
				float width=pos.getAttributef("width");
				FD_DATA(fix,type)=COVER;
				Vector2dVector vec1(count);
				Vector2dVector vec2(count);
				vector<float>length1(count);
				vector<float>length2(count);
				float length_a1=0,length_a2=0;
				for(int q=0; q<count; q++) {
					XMLNode point=pos.getChildNode("point",q);
					vec1[q].x=point.getAttributef("x");
					vec1[q].y=point.getAttributef("y");
					if(q!=0)length_a1+=b2Distance(vec1[q],vec1[q-1]);
					length1[q]=length_a1;
				}
				for(int e=0; e<count; e++) {
					b2Vec2 prev=(e==0)			?	2*vec1[e]-vec1[e+1]	:	vec1[e-1];
					b2Vec2 next=(e==count-1)	?	2*vec1[e]-vec1[e-1]	:	vec1[e+1];
					vec2[e]=bis(prev-vec1[e],next-vec1[e],width)+vec1[e];
					if(e!=0)length_a2+=b2Distance(vec2[e],vec2[e-1]);
					length2[e]=length_a2;
				}
				for(int q=1; q<count; q++) {
					b2FixtureDef fix2=fix;
					b2PolygonShape shape;
					b2Vec2 v[4];
					b2Vec2 tex[4];
					if(q==0 || bigger_angle(vec1[q]-vec1[q-1],vec1[q-1]-vec1[q-2])) {
						v[0]=vec1[q-1];
						v[3]=v[0]-point2_per(vec1[q-1],vec1[q],width);
						tex[0]=b2Vec2(length1[q-1],width);
						tex[3]=b2Vec2(length1[q-1],0);
					} else {
						v[0]=vec2[q-1];
						v[3]=v[0]+point2_per(vec2[q-1],vec2[q],width);
						tex[0]=b2Vec2(length2[q-1],0);
						tex[3]=b2Vec2(length2[q-1],width);
					}
					if(q==count-1 || !bigger_angle(vec1[q-1]-vec1[q],vec1[q]-vec1[q+1])) {
						v[1]=vec1[q];
						v[2]=v[1]-point2_per(vec1[q-1],vec1[q],width);
						tex[1]=b2Vec2(length1[q],width);
						tex[2]=b2Vec2(length1[q],0);
					} else {
						v[1]=vec2[q];
						v[2]=v[1]+point2_per(vec2[q-1],vec2[q],width);
						tex[1]=b2Vec2(length2[q],0);
						tex[2]=b2Vec2(length2[q],width);
					}
					shape.Set(v,4);
					shape.big_polygon=new b2Vec2[5];
					shape.b_count=5;
					for(int e=0; e<4; e++)
						for(int i=0; i<4; i++)
							if(shape.m_vertices[i]==v[e])
								shape.big_polygon[i]=tex[e];
					shape.big_polygon[4]=b2Vec2(max(length_a1,length_a2),width);
					fix2.shape=&shape;
					body->CreateFixture(&fix2);
				}
			}
		}
	}
	return body;
}
void set_bds(b2JointDef *j,XMLNode &node,string id1,string id2,Entity *ent=0) {
	if(id1==id2)throw string("body \"" + id1 + "\"cannot be declared twice");
	if(ent) {
		j->bodyA=ent->get_body(id1);
		j->bodyB=ent->get_body(id2);
	} else {
		j->bodyA=get_body(id1);
		j->bodyB=get_body(id2);
	}
	j->collideConnected=node.getAttributei("collide");
	j->userData=new b2JointData;
}
b2Joint *read_joint(XMLNode jn,string &id,b2Vec2 delta,Entity *ent) {
	string type=jn.getAttribute("type");
	XMLNode pos=jn.getChildNode("position");
	XMLNode phs=jn.getChildNode("physic");
	XMLNode con=jn.getChildNode("connected");
	string id1=con.getAttribute("id1");
	string id2=con.getAttribute("id2");
	id=jn.getAttribute("id");
	b2Joint *j;
	if(type=="WeldJoint") {
		b2WeldJointDef joint;
		joint.Initialize(joint.bodyA,joint.bodyB,
						 b2Vec2(pos.getAttributef("x"),pos.getAttributef("y"))+delta);
		set_bds(&joint,con,id1,id2,ent);
		joint.stiffness=phs.getAttributef("stiffness");
		joint.damping=phs.getAttributef("damping");
		j=world->CreateJoint(&joint);
	} else if(type=="RevoluteJoint") {
		b2RevoluteJointDef joint;
		set_bds(&joint,con,id1,id2,ent);
		joint.Initialize(joint.bodyA,joint.bodyB,
						 b2Vec2(pos.getAttributef("x"),pos.getAttributef("y"))+delta);
		joint.enableLimit=phs.getAttributei("limit");
		joint.enableMotor=phs.getAttributei("motor");
		if(joint.enableLimit) {
			joint.lowerAngle=phs.getAttributef("lower");
			joint.upperAngle=phs.getAttributef("upper");
		}
		if(joint.enableMotor) {
			joint.maxMotorTorque=phs.getAttributef("max_torque");
			joint.motorSpeed=phs.getAttributef("speed");
		}
		j=world->CreateJoint(&joint);
	} else if(type=="GearJoint") {
		b2GearJointDef joint;
		joint.userData=new b2JointData;
		joint.collideConnected=con.getAttributei("collide");
		joint.ratio=phs.getAttributef("ratio");
		b2Joint *j1=joints[id1];
		b2Joint *j2=joints[id2];
		if(!j1)throw string("\""+id1="\"is not a joint");
		if(!j2)throw string("\""+id2="\"is not a joint");
		joint.joint1=j1;
		joint.joint2=j2;
		if(j1->GetBodyA()!=j2->GetBodyB()) {
			joint.bodyA=j1->GetBodyA();
			joint.bodyB=j1->GetBodyB();
		} else if(j1->GetBodyA()!=j2->GetBodyA()) {
			joint.bodyA=j1->GetBodyA();
			joint.bodyB=j1->GetBodyA();
		} else if(j1->GetBodyB()!=j2->GetBodyB()) {
			joint.bodyA=j1->GetBodyB();
			joint.bodyB=j1->GetBodyB();
		} else if(j1->GetBodyB()!=j2->GetBodyA()) {
			joint.bodyA=j1->GetBodyB();
			joint.bodyB=j1->GetBodyA();
		}
		j=(b2GearJoint*)world->CreateJoint(&joint);
	} else if(type=="PrismaticJoint") {
		b2PrismaticJointDef joint;
		set_bds(&joint,con,id1,id2,ent);
		float angle=pos.getAttributef("angle")+M_PI;
		joint.Initialize(joint.bodyA,joint.bodyB,
						 b2Vec2(pos.getAttributef("x"),pos.getAttributef("y"))+delta,
						 b2Vec2(cos(angle),sin(angle)));
		joint.enableLimit=phs.getAttributei("limit");
		joint.enableMotor=phs.getAttributei("motor");
		if(joint.enableLimit) {
			joint.lowerTranslation=phs.getAttributef("lower");
			joint.upperTranslation=phs.getAttributef("upper");
		}
		if(joint.enableMotor) {
			joint.maxMotorForce=phs.getAttributef("max_force");
			joint.motorSpeed=phs.getAttributef("speed");
		}
		j=world->CreateJoint(&joint);
	} else if(type=="DistanceJoint") {
		b2DistanceJointDef joint;
		set_bds(&joint,con,id1,id2,ent);
		float x1=pos.getAttributef("x1");
		float x2=pos.getAttributef("x2");
		float y1=pos.getAttributef("y1");
		float y2=pos.getAttributef("y2");
		joint.Initialize(joint.bodyA,joint.bodyB, delta+b2Vec2(x1,y1), delta+b2Vec2(x2,y2));
		joint.length=hypot(x2-x1,y2-y1);
		joint.maxLength=joint.length+pos.getAttributef("max");
		joint.minLength=joint.length+pos.getAttributef("min");
		joint.stiffness=phs.getAttributef("stiffness");
		joint.damping=phs.getAttributef("damping");
		j=world->CreateJoint(&joint);
	} else if(type=="PulleyJoint") {
		b2PulleyJointDef joint;
		set_bds(&joint,con,id1,id2,ent);
		joint.Initialize(joint.bodyA,joint.bodyB,
						 b2Vec2(pos.getAttributef("x3"),pos.getAttributef("y3"))+delta,
						 b2Vec2(pos.getAttributef("x4"),pos.getAttributef("y4"))+delta,
						 b2Vec2(pos.getAttributef("x1"),pos.getAttributef("y2"))+delta,
						 b2Vec2(pos.getAttributef("x2"),pos.getAttributef("y2"))+delta,
						 phs.getAttributef("ratio"));
		j=world->CreateJoint(&joint);
	}
	J_DATA(j,id)=id;
	return j;
}
void open_file(string path) {
	XMLNode lvl=open_xml(path.c_str(),"level");
	world=new b2World(b2Vec2(0,9.8));
	{
		//backgroung
		XMLNode bgr=lvl.getChildNode("background");
		background=bgr.getAttribute("img");
	}
	{
		//bodies
		XMLNode bds=lvl.getChildNode("bodies");
		int bodies_count=bds.getAttributei("count");
		bodies.clear();
		for(int q=0; q<bodies_count; q++) {
			XMLNode bd=bds.getChildNode("body",q);
			b2Body *b=read_body(bd);
			string id=bd.getAttribute("id");
			bodies[id]=b;
			B_DATA(b,id)=id;
		}
	}
	{
		//joints
		XMLNode js=lvl.getChildNode("joints");
		int joints_count=js.getAttributei("count");
		joints.clear();
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")!="GearJoint") {
				string str;
				b2Joint *j=read_joint(ch,str);
				joints[str]=j;
			}
		}
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")=="GearJoint") {
				string str;
				b2Joint *j=read_joint(ch,str);
				joints[str]=j;
			}
		}
	}
	{
		//entities
		XMLNode ens=lvl.getChildNode("entities");
		int count=ens.getAttributei("count");
		entities.clear();
		for(int q=0; q<count; q++) {
			XMLNode en=ens.getChildNode("entity",q);
			XMLNode pos=en.getChildNode("position");
			float x=pos.getAttributef("x");
			float y=pos.getAttributef("y");
			string id=en.getAttribute("id");
			string type=en.getAttribute("type");
			entities[id]=new Entity(type,x,y);
			entities[id]->id=id;
		}
	}
	load_textures();
}
void close_level() {
	destroy_all();
	for(auto &j : joints)
		world->DestroyJoint(j.second);
	for(auto &b : bodies)
		world->DestroyBody(b.second);
	joints.clear();
	bodies.clear();
	entities.clear();
	lua::clear_loaded_list();
	effect::effects.clear();
	effect::loaded.clear();
	text::clear_text();
	if(world)delete world;
	lua::quit();
}
void load_level(string name,bool n) {
	info_log("Level: "+name);
	levelname=name;
	close_level();
	open_file(prefix+"levels/"+name+".xml");
	info_log("Loaded objects");
	lua::init_level(name,n);
	info_log("Lua inited");
	info_log("Level loaded successfully");
	if(!interface.mainmenu.shown)
		interface.levelchooser.open_level(name);
}
