#include "level.hpp"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "physic.hpp"
#include "utility.hpp"
#include "lua.hpp"
#include "sdl.hpp"
#include "triangulate.hpp"
using namespace std;

unsigned short int levelw=20;
unsigned short int levelh=20;
vector<b2Body*>bodies;
vector<b2Joint*>joints;
b2World world(b2Vec2(0,9.8f));
float startx,starty,endx,endy;

b2Body* read_body(XMLNode bd) {
	int shapes_count=stoi(bd.getAttribute("shapes"));
	if(shapes_count==0)
		throw string("Body \""+(string)bd.getAttribute("id")+"\" is empty");
	b2BodyDef def;
	b2Body *body;
	def.userData=new b2BodyData;
	BD_DATA(def,id)=bd.getAttribute("id");
	def.position.Set(stof(bd.getAttribute("x")),
					 stof(bd.getAttribute("y")));
	{
		XMLNode phs=bd.getChildNode("physic");
		def.fixedRotation=stoi(phs.getAttribute("fixed_rotation"));
		def.bullet=stoi(phs.getAttribute("bullet"));
		def.gravityScale=stof(phs.getAttribute("gravity_scale"));
	}
	{
		string str=bd.getAttribute("type");
		if(str=="static")           def.type=b2BodyType::b2_staticBody;
		else if(str=="dynamic")     def.type=b2BodyType::b2_dynamicBody;
		else if(str=="kinematic")   def.type=b2BodyType::b2_kinematicBody;
	}
	body=world.CreateBody(&def);
	for(int j=0; j<shapes_count; j++) {
		XMLNode sh=bd.getChildNode("shape",j);
		b2FixtureDef fix;
		fix.userData=new b2FixtureData;
		FD_DATA(fix,id)      =sh.getAttribute("id");
		FD_DATA(fix,texture) =sh.getAttribute("texture");
		{
			//pos
			string str=sh.getAttribute("pos");
			if(str=="background")        FD_DATA(fix,pos)=TBGR;
			else if(str=="physic")       FD_DATA(fix,pos)=TMGR;
			else if(str=="foreground")   FD_DATA(fix,pos)=TFGR;
			fix.isSensor=(str!="physic");
		}
		{
			//physic
			XMLNode phs=sh.getChildNode("physic");
			fix.density=    stof(phs.getAttribute("density"));
			fix.friction=   stof(phs.getAttribute("friction"));
			fix.restitution=stof(phs.getAttribute("restitution"));
		}
		{
			//position
			XMLNode pos=sh.getChildNode("position");
			string str=sh.getAttribute("type");
			if(str=="Square") {
				b2PolygonShape shape;
				float r=stof(pos.getAttribute("r"));
				shape.SetAsBox(r,r,b2Vec2(
								   stof(pos.getAttribute("x")),
								   stof(pos.getAttribute("y"))),0);
				fix.shape=&shape;
				FD_DATA(fix,type)=SQUARE;
				body->CreateFixture(&fix);
			} else if(str=="Circle") {
				b2CircleShape shape;
				shape.m_radius=stof(pos.getAttribute("r"));
				shape.m_p.Set( stof(pos.getAttribute("x")),
							   stof(pos.getAttribute("y")));
				fix.shape=&shape;
				FD_DATA(fix,type)=CIRCLE;
				body->CreateFixture(&fix);
			} else if(str=="Rect") {
				b2PolygonShape shape;
				float xp1=stof(pos.getAttribute("x1"));
				float yp1=stof(pos.getAttribute("y1"));
				float xp2=stof(pos.getAttribute("x2"));
				float yp2=stof(pos.getAttribute("y2"));
				shape.SetAsBox(abs(xp2-xp1)/2,abs(yp2-yp1)/2,b2Vec2((xp1+xp2)/2,(yp1+yp2)/2),0);
				fix.shape=&shape;
				FD_DATA(fix,type)=RECT;
				body->CreateFixture(&fix);
			} else if(str=="Line") {
				b2EdgeShape shape;
				float x1=stof(pos.getAttribute("x1"));
				float y1=stof(pos.getAttribute("y1"));
				float x2=stof(pos.getAttribute("x2"));
				float y2=stof(pos.getAttribute("y2"));
				shape.SetTwoSided(b2Vec2(x1,y1),b2Vec2(x2,y2));
				fix.shape=&shape;
				FD_DATA(fix,type)=LINE;
				body->CreateFixture(&fix);
			} else if(str=="Polygon") {
				int count=stoi(pos.getAttribute("point_count"));
				FD_DATA(fix,type)=POLYGON;
				Vector2dVector vec(count),result;
				b2Vec2 *big=new b2Vec2[count];

				for(int e=0; e<count; e++) {
					XMLNode point=pos.getChildNode("point",e);
					big[e].x=vec[e].x=stof(point.getAttribute("x"));
					big[e].y=vec[e].y=stof(point.getAttribute("y"));
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
			}
		}
	}
	return body;
}
void set_bds(b2JointDef *j,XMLNode &node,string id1,string id2) {
	j->bodyA=get_body(id1);
	j->bodyB=get_body(id2);
	j->collideConnected=stoi(node.getAttribute("collide"));
	j->userData=new b2JointData;
}
b2Joint *read_joint(XMLNode jn) {
	string type=jn.getAttribute("type");
	XMLNode pos=jn.getChildNode("position");
	XMLNode phs=jn.getChildNode("physic");
	XMLNode con=jn.getChildNode("connected");
	string id1=con.getAttribute("id1");
	string id2=con.getAttribute("id2");
	b2Joint *j;
	if(type=="WeldJoint") {
		b2WeldJointDef joint;
		joint.Initialize(get_body(id1),get_body(id2),
						 b2Vec2(stof(pos.getAttribute("x")),stof(pos.getAttribute("y"))));
		set_bds(&joint,con,id1,id2);
		joint.stiffness=stof(phs.getAttribute("stiffness"));
		joint.damping=stof(phs.getAttribute("damping"));
		JD_DATA(joint,id)=jn.getAttribute("id");
		j=world.CreateJoint(&joint);
	} else if(type=="RevoluteJoint") {
		b2RevoluteJointDef joint;
		set_bds(&joint,con,id1,id2);
		joint.Initialize(get_body(id1),get_body(id2),
						 b2Vec2(stof(pos.getAttribute("x")),stof(pos.getAttribute("y"))));
		joint.enableLimit=stoi(phs.getAttribute("limit"));
		joint.enableMotor=stoi(phs.getAttribute("motor"));
		if(joint.enableLimit) {
			joint.lowerAngle=stof(phs.getAttribute("lower"));
			joint.upperAngle=stof(phs.getAttribute("upper"));
		}
		if(joint.enableMotor) {
			joint.maxMotorTorque=stof(phs.getAttribute("max_torque"));
			joint.motorSpeed=stof(phs.getAttribute("speed"));
		}
		JD_DATA(joint,id)=jn.getAttribute("id");
		j=world.CreateJoint(&joint);
	} else if(type=="GearJoint") {
		b2GearJointDef joint;
		joint.userData=new b2JointData;
		joint.collideConnected=stoi(con.getAttribute("collide"));
		joint.ratio=stof(phs.getAttribute("ratio"));
		b2Joint *j1=0,*j2=0;
		for(int q=0; q<joints.size(); q++) {
			if(joints[q]) {
				string str=J_DATA(joints[q],id);
				if(J_DATA(joints[q],id)==id1)
					j1=joints[q];
				if(J_DATA(joints[q],id)==id2)
					j2=joints[q];
			}
		}
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
		JD_DATA(joint,id)=jn.getAttribute("id");
		j=(b2GearJoint*)world.CreateJoint(&joint);
	} else if(type=="PrismaticJoint") {
		b2PrismaticJointDef joint;
		set_bds(&joint,con,id1,id2);
		float angle=stof(pos.getAttribute("angle"));
		joint.Initialize(get_body(id1),get_body(id2),
						 b2Vec2(stof(pos.getAttribute("x")),stof(pos.getAttribute("y"))),
						 b2Vec2(cos(angle),sin(angle)));
		joint.enableLimit=stoi(phs.getAttribute("limit"));
		joint.enableMotor=stoi(phs.getAttribute("motor"));
		if(joint.enableLimit) {
			joint.lowerTranslation=stof(phs.getAttribute("lower"));
			joint.upperTranslation=stof(phs.getAttribute("upper"));
		}
		if(joint.enableMotor) {
			joint.maxMotorForce=stof(phs.getAttribute("max_force"));
			joint.motorSpeed=stof(phs.getAttribute("speed"));
		}
		JD_DATA(joint,id)=jn.getAttribute("id");
		j=world.CreateJoint(&joint);
	} else if(type=="DistanceJoint") {
		b2DistanceJointDef joint;
		set_bds(&joint,con,id1,id2);
		float x1=stof(pos.getAttribute("x1"));
		float x2=stof(pos.getAttribute("x2"));
		float y1=stof(pos.getAttribute("y1"));
		float y2=stof(pos.getAttribute("y2"));
		joint.Initialize(joint.bodyA,joint.bodyB, {x1,y1}, {x2,y2});
		joint.length=hypot(x2-x1,y2-y1);
		joint.maxLength=joint.length+stof(pos.getAttribute("max"));
		joint.minLength=joint.length+stof(pos.getAttribute("min"));
		joint.stiffness=stof(phs.getAttribute("stiffness"));
		joint.damping=stof(phs.getAttribute("damping"));
		JD_DATA(joint,id)=jn.getAttribute("id");
		j=world.CreateJoint(&joint);
	} else if(type=="PulleyJoint") {
		b2PulleyJointDef joint;
		set_bds(&joint,con,id1,id2);
		joint.Initialize(joint.bodyA,joint.bodyB,
		{stof(pos.getAttribute("x3")),stof(pos.getAttribute("y3"))},
		{stof(pos.getAttribute("x4")),stof(pos.getAttribute("y4"))},
		{stof(pos.getAttribute("x1")),stof(pos.getAttribute("y2"))},
		{stof(pos.getAttribute("x2")),stof(pos.getAttribute("y2"))},
		stof(phs.getAttribute("ratio")));
		JD_DATA(joint,id)=jn.getAttribute("id");
		j=world.CreateJoint(&joint);
	}
	return j;
}
static int status=0;
void open_file(string path) {
#define DEBUG {printf("Status: %d\n",status++);}
	XMLNode lvl=XMLNode::openFileHelper(path.c_str(),"level");
	{
		//backgroung
		XMLNode bgr=lvl.getChildNode("background");
		load_background(bgr.getAttribute("img"));
	}
	{
		//start
		XMLNode bgr=lvl.getChildNode("start");
		startx=stof(bgr.getAttribute("x"));
		starty=stof(bgr.getAttribute("y"));
	}
	{
		//end
		XMLNode bgr=lvl.getChildNode("end");
		endx=stof(bgr.getAttribute("x"));
		endy=stof(bgr.getAttribute("y"));
	}
	{
		//bodies
		XMLNode bds=lvl.getChildNode("bodies");
		int bodies_count=stoi(bds.getAttribute("count"));
		bodies.resize(bodies_count);
		for(int q=0; q<bodies_count; q++) {
			bodies[q]=read_body(bds.getChildNode("body",q));
		}
	}
	{
		//joints
		XMLNode js=lvl.getChildNode("joints");
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
	load_textures();
}

void close_level() {
	destroy_textures();
	if(!mainscript_enabled)return;
	lua_quit();
}
void load_level(string name) {
	try {
		open_file("levels/"+name+".xml");
		lua_init(name);
	} catch(XMLError er) {
		panic("Error loading \""+name+"\" level",XMLNode::getError(er));
	} catch(string er) {
		panic("Error loading \""+name+"\" level",er);
	}
}
