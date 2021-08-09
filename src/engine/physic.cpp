#include "physic.hpp"
#include "level.hpp"
#include "utility.hpp"
#include "sdl.hpp"
#include "main.hpp"
#include "lua.hpp"
#include "camera.hpp"
int velocity_iterations=10;
int position_iterations=10;
using namespace std;

b2Body *create_body_winit(string type,string id) {
	XMLNode bd=open_xml((prefix+"bodies/"+type+".xml").c_str(),"body");
	b2Body *body=read_body(bd, {0,0},1);
	load_body_textures(body);
	if(id=="")id=free_id();
	bodies[id]=body;
	B_DATA(body,id)=id;
	lua::init_body(body,0);
	body->m_userData->created=true;
	return body;
}
b2Body *create_body(string type,string id,float x,float y) {
	XMLNode bd=open_xml((prefix+"bodies/"+type+".xml").c_str(),"body");
	b2Body *body=read_body(bd, {x,y},1);
	load_body_textures(body);
	if(id=="")id=free_id();
	bodies[id]=body;
	B_DATA(body,id)=id;
	lua::create_body_userdata(body);
	lua::init_body(body);
	body->m_userData->created=true;
	return body;
}
Entity *create_entity_winit(string type,string id) {
	Entity *ent=new Entity(type,0,0);
	load_entity_textures(ent);
	if(id=="")id=free_entity_id();
	entities[id]=ent;
	ent->id=id;
	ent->created=true;
	lua::init_entity(ent,0);
	return ent;
}
Entity *create_entity(string type,string id,float x,float y) {
	Entity *ent=new Entity(type,x,y);
	load_entity_textures(ent);
	if(id=="")id=free_entity_id();
	entities[id]=ent;
	ent->id=id;
	lua::create_entity_userdata(ent);
	lua::init_entity(ent);
	ent->created=true;
	return ent;
}
void set_gravity(float x,float y) {
	world->SetGravity({x,y});
}
void destroy_body(b2Body *body) {
	for(auto &b : bodies) {
		if(b.second==body) {
			bodies.erase(b.first);
			world->DestroyBody(body);
			return;
		}
	}
}

void destroy_joint(b2Joint *joint) {
	for(auto &j : joints) {
		if(j.second==joint) {
			joints.erase(j.first);
			world->DestroyJoint(joint);
			return;
		}
	}
}

void destroy_entity(Entity *entity) {
	for(auto &e : entities) {
		if(e.second==entity) {
			//delete entity;
			entities.erase(e.first);
			return;
		}
	}
}
bool bb_all_collide(b2Body *b1,b2Body *b2) {
	if(!b1 || !b2)return false;
	for(b2ContactEdge *c=b1->GetContactList(); c; c=c->next) {
		if(c->other==b2 && c->contact->IsTouching())
			return true;
	}
	return false;
}
b2Body *eb_all_collide(Entity *entity,b2Body *b) {
	if(!entity || !b)return nullptr;
	for(auto &b2 : entity->bodies) {
		if(bb_all_collide(b2.second,b))
			return b2.second;
	}
	return nullptr;
}
bool ee_all_collide(Entity *e1,Entity *e2) {
	if(!e1 || !e2)return false;
	for(auto &b1 : e1->bodies) {
		if(eb_all_collide(e2,b1.second))
			return true;
	}
	return false;
}
b2Body *lb_all_collide(b2Body *body) {
	if(!body)return nullptr;
	for(auto &b1 : bodies) {
		if(bb_all_collide(b1.second,body))
			return b1.second;
	}
	return nullptr;
}
b2Body *le_all_collide(Entity *e) {
	if(!e)return nullptr;
	for(auto &body : e->bodies) {
		if(lb_all_collide(body.second))
			return body.second;
	}
	return nullptr;
}

Entity *sb_all_collide(b2Body *body) {
	if(!body)return nullptr;
	for(auto &e : entities) {
		if(eb_all_collide(e.second,body))
			return e.second;
	}
	return nullptr;
}

bool bb_collide(b2Body *b1,b2Body *b2) {
	if(!b1 || !b2)return false;
	for(b2ContactEdge *c=b1->GetContactList(); c; c=c->next) {
		if(c->other==b2 && c->contact->IsTouching()
				&& !c->contact->m_fixtureA->IsSensor()
				&& !c->contact->m_fixtureB->IsSensor()
				&& c->contact->IsEnabled())
			return true;
	}
	return false;
}
b2Body *eb_collide(Entity *entity,b2Body *b) {
	if(!entity || !b)return nullptr;
	for(auto &b2 : entity->bodies) {
		if(bb_collide(b2.second,b))
			return b2.second;
	}
	return nullptr;
}
bool ee_collide(Entity *e1,Entity *e2) {
	if(!e1 || !e2)return false;
	for(auto &b1 : e1->bodies) {
		if(eb_collide(e2,b1.second))
			return true;
	}
	return false;
}
b2Body *lb_collide(b2Body *body) {
	if(!body)return nullptr;
	for(auto &b1 : bodies) {
		if(bb_collide(b1.second,body))
			return b1.second;
	}
	return nullptr;
}
b2Body *le_collide(Entity *e) {
	if(!e)return nullptr;
	for(auto &body : e->bodies) {
		if(lb_collide(body.second))
			return body.second;
	}
	return nullptr;
}
Entity *sb_collide(b2Body *body) {
	if(!body)return nullptr;
	for(auto &e : entities) {
		if(eb_collide(e.second,body))
			return e.second;
	}
	return nullptr;
}

static b2Vec2 compute_centroid(std::vector<b2Vec2> vs, float& area) {
	int count = (int)vs.size();
	b2Assert(count >= 3);

	b2Vec2 c;
	c.Set(0.0f, 0.0f);
	area = 0.0f;
	b2Vec2 pRef(0.0f, 0.0f);

	const float inv3 = 1.0f / 3.0f;

	for (int32 i = 0; i < count; ++i) {
		b2Vec2 p1 = pRef;
		b2Vec2 p2 = vs[i];
		b2Vec2 p3 = i + 1 < count ? vs[i+1] : vs[0];

		b2Vec2 e1 = p2 - p1;
		b2Vec2 e2 = p3 - p1;

		float D = b2Cross(e1, e2);

		float triangleArea = 0.5f * D;
		area += triangleArea;
		c += triangleArea * inv3 * (p1 + p2 + p3);
	}
	if (area > b2_epsilon)
		c *= 1.0f / area;
	else
		area = 0;
	return c;
}

static bool inside(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 p) {
	return (cp2.x-cp1.x)*(p.y-cp1.y) > (cp2.y-cp1.y)*(p.x-cp1.x);
}

static b2Vec2 intersection(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 s, b2Vec2 e) {
	b2Vec2 dc( cp1.x - cp2.x, cp1.y - cp2.y );
	b2Vec2 dp( s.x - e.x, s.y - e.y );
	float n1 = cp1.x * cp2.y - cp1.y * cp2.x;
	float n2 = s.x * e.y - s.y * e.x;
	float n3 = 1.0 / (dc.x * dp.y - dc.y * dp.x);
	return b2Vec2( (n1*dp.x - n2*dc.x) * n3, (n1*dp.y - n2*dc.y) * n3);
}

static vector<b2Vec2>create_polygon(b2Fixture *fixture) {
	b2CircleShape *shape=TYPE(b2CircleShape*,fixture->GetShape());
	vector<b2Vec2>vec;
	for(int q=0; q<CIRCLE_QUALITY; q++) {
		b2Vec2 v(shape->m_p.x+cos(2*M_PI/(CIRCLE_QUALITY)*q)*shape->m_radius,
				 shape->m_p.y+sin(2*M_PI/(CIRCLE_QUALITY)*q)*shape->m_radius);
		vec.push_back(fixture->GetBody()->GetWorldPoint(v));
	}
	return vec;
}

static bool find_intersection_of_fixtures(b2Fixture* fA, b2Fixture* fB, std::vector<b2Vec2>& output) {
	if ((fA->GetShape()->GetType() != b2Shape::e_polygon &&
			fA->GetShape()->GetType() != b2Shape::e_circle) ||
			(fB->GetShape()->GetType() != b2Shape::e_polygon &&
			 fB->GetShape()->GetType() != b2Shape::e_circle))
		return false;
	if(fA->GetShape()->GetType() == b2Shape::e_polygon) {
		b2PolygonShape* polyA = (b2PolygonShape*)fA->GetShape();
		for (int i = 0; i < polyA->m_count; i++)
			output.push_back(fA->GetBody()->GetWorldPoint( polyA->m_vertices[i]));
	} else
		output=create_polygon(fA);
	std::vector<b2Vec2> clip;
	if(fB->GetShape()->GetType() == b2Shape::e_polygon) {
		b2PolygonShape* polyB = (b2PolygonShape*)fB->GetShape();
		for (int i = 0; i < polyB->m_count; i++)
			clip.push_back(fB->GetBody()->GetWorldPoint(polyB->m_vertices[i]));
	} else
		clip=create_polygon(fB);

	b2Vec2 cp1 = clip[clip.size()-1];
	for (int j = 0; j < clip.size(); j++) {
		b2Vec2 cp2 = clip[j];
		if ( output.empty() )
			return false;
		std::vector<b2Vec2> input = output;
		output.clear();
		b2Vec2 s = input[input.size() - 1];
		for (int i = 0; i < input.size(); i++) {
			b2Vec2 e = input[i];
			if (inside(cp1, cp2, e)) {
				if (!inside(cp1, cp2, s)) {
					output.push_back( intersection(cp1, cp2, s, e) );
				}
				output.push_back(e);
			} else if (inside(cp1, cp2, s)) {
				output.push_back( intersection(cp1, cp2, s, e) );
			}
			s = e;
		}
		cp1 = cp2;
	}

	return !output.empty();
}

void update_fluid() {
	for(b2Contact *c=world->GetContactList(); c; c=c->GetNext()) {
		if((c->m_flags & b2Contact::e_fluidFlag)==b2Contact::e_fluidFlag) {
			b2Fixture *fixtureA=c->m_fixtureA;
			b2Fixture *fixtureB=c->m_fixtureB;
			if(fixtureB->IsSensor()==2)
				swap(fixtureA,fixtureB);
			float density = fixtureA->GetDensity();
			std::vector<b2Vec2> intersectionPoints;
			if (find_intersection_of_fixtures(fixtureA, fixtureB, intersectionPoints)) {
				float area = 0;
				b2Vec2 centroid = compute_centroid( intersectionPoints, area);
				float displacedMass = fixtureA->GetDensity() * area;
				b2Vec2 buoyancyForce = displacedMass * -world->GetGravity();
				fixtureB->GetBody()->ApplyForce(buoyancyForce, centroid,1);
				float dragMod = 0.25;
				float liftMod = 0.25;
				float maxDrag = 2000;
				float maxLift = 500;
				for (int i = 0; i < intersectionPoints.size(); i++) {
					b2Vec2 v0 = intersectionPoints[i];
					b2Vec2 v1 = intersectionPoints[(i+1)%intersectionPoints.size()];
					b2Vec2 midPoint = 0.5f * (v0+v1);
					b2Vec2 velDir = fixtureB->GetBody()->GetLinearVelocityFromWorldPoint(midPoint) -
									fixtureA->GetBody()->GetLinearVelocityFromWorldPoint(midPoint);
					float vel = velDir.Normalize();
					b2Vec2 edge = v1 - v0;
					float edgeLength = edge.Normalize();
					b2Vec2 normal = b2Cross(-1,edge);
					float dragDot = b2Dot(normal, velDir);
					if ( dragDot < 0 )
						continue;
					float dragMag = dragDot * dragMod * edgeLength * density * vel * vel;
					dragMag = b2Min( dragMag, maxDrag );
					b2Vec2 dragForce = dragMag * -velDir;
					fixtureB->GetBody()->ApplyForce(dragForce, midPoint,1);
					float liftDot = b2Dot(edge, velDir);
					float liftMag =  dragDot * liftDot * liftMod * edgeLength * density * vel * vel;
					liftMag = b2Min( liftMag, maxLift );
					b2Vec2 liftDir = b2Cross(1,velDir);
					b2Vec2 liftForce = liftMag * liftDir;
					fixtureB->GetBody()->ApplyForce(liftForce, midPoint,1);
				}
			}
		}
	}
}
b2Body *screen;
void update_bodies(){
	b2PolygonShape *shape=(b2PolygonShape*)screen->GetFixtureList()->GetShape();
	shape->SetAsBox(SW/zoom/2,SH/zoom/2);
	screen->SetTransform({worldx(SW/2),worldy(SH/2)},screen->GetAngle());
}
void init_bodies(){
	b2BodyDef body;
	body.type=b2_dynamicBody;
	body.position.Set(worldx(SW/2),worldy(SH/2));
	screen=world->CreateBody(&body);
	b2PolygonShape shape;
	shape.SetAsBox(SW/zoom/2,SH/zoom/2);
	b2FixtureDef def;
	def.isSensor=1;
	def.shape=&shape;
	screen->CreateFixture(&def);
}
bool on_screen_b(b2Body *b){
	for(b2ContactEdge *c=b->GetContactList(); c; c=c->next) {
		if(c->contact->IsTouching() &&c->other==screen)
			return true;
	}
	return false;
}
bool on_screen_f(b2Fixture *f){
	for(b2ContactEdge *c=screen->GetContactList(); c; c=c->next) {
		b2Contact *cont=c->contact;
		if((cont->m_fixtureA==f || cont->m_fixtureB==f))return true;
	}
	return false;
}
