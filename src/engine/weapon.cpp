#include "weapon.hpp"
#include "main.hpp"
#include "sdl.hpp"
#include "physic.hpp"
#include "level.hpp"
#include "lua.hpp"
#include "utility.hpp"
#include <fstream>
#include <cmath>
map<string,Weapon>weapons;
map<string,Bullet>bullets;
const int num_rays=100;
void Bullet::add(int c) {
	if(count<0)return;
	count+=c;
	if(count>max)count=max;
}
void Bullet::del(int c) {
	if(count<0)return;
	count-=c;
	if(count<0)count=0;
}
bool Bullet::full() {
	return (count==max || count<0 || max<0);
}

b2Body *simple_bullet(Entity *e, string type, float impulse) {
	float &angle=e->weapon.angle;
	b2Body *bullet=create_body(type,"",e->getx(),e->gety());
	bullet->SetAngle(angle);
	bullet->SetLinearVelocity(e->get_first()->GetLinearVelocity());
	bullet->CenterImpulse(cos(angle)*impulse,sin(angle)*impulse);
	e->get_first()->CenterImpulse(-cos(angle)*impulse,-sin(angle)*impulse);
	return bullet;
}
struct RayCastClosestCallback : public b2RayCastCallback {
	RayCastClosestCallback() {
		m_hit = false;
	}

	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float) override {
		m_hit = true;
		m_point = point;
		m_normal = normal;
		body=fixture->GetBody();
		if(fixture->IsSensor())
			return 1.0f;
		return 0.0f;
	}

	bool m_hit;
	b2Vec2 m_point;
	b2Vec2 m_normal;
	b2Body *body;
};
void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower) {
	b2Vec2 blastDir = applyPoint - blastCenter;
	float distance = blastDir.Normalize();
	if ( distance ==0 )
		return;

	float invDistance = 1 / distance;
	float impulseMag = blastPower * invDistance * invDistance;
	body->ApplyLinearImpulse(impulseMag * blastDir, applyPoint,1);
}

void explosion_callbacks(float x,float y,float radius,float power,luabridge::LuaRef c_entity, luabridge::LuaRef c_body) {
	b2Vec2 center(x,y);
	for (int i = 0; i < num_rays; i++) {
		float angle = (i / (float)num_rays) * M_PI * 2;
		b2Vec2 rayDir( sinf(angle), cosf(angle) );
		b2Vec2 rayEnd = center + radius * rayDir;
		RayCastClosestCallback callback;
		world->RayCast(&callback, center, rayEnd);
		if ( callback.body ) {
			applyBlastImpulse(callback.body, center, callback.m_point, (power / (float)num_rays));
			Entity *ent=whois(callback.body);
			if(ent && c_entity.isFunction())
				c_entity(ent,sqrt((callback.m_point-center).Length()));
			if(c_body.isFunction())
				c_body(callback.body,sqrt((callback.m_point-center).Length()));
		}
	}
}

void explosion(float x,float y,float radius,float power) {
	b2Vec2 center(x,y);
	for (int i = 0; i < num_rays; i++) {
		float angle = (i / (float)num_rays) * M_PI * 2;
		b2Vec2 rayDir( sinf(angle), cosf(angle) );
		b2Vec2 rayEnd = center + radius * rayDir;
		RayCastClosestCallback callback;
		world->RayCast(&callback, center, rayEnd);
		if ( callback.body )
			applyBlastImpulse(callback.body, center, callback.m_point, (power / (float)num_rays));
	}
}

void raycast_callbacks(float x,float y,float radius,luabridge::LuaRef c_entity, luabridge::LuaRef c_body) {
	b2Vec2 center(x,y);
	for (int i = 0; i < num_rays; i++) {
		float angle = (i / (float)num_rays) * M_PI * 2;
		b2Vec2 rayDir( sinf(angle), cosf(angle) );
		b2Vec2 rayEnd = center + radius * rayDir;
		RayCastClosestCallback callback;
		world->RayCast(&callback, center, rayEnd);
		if ( callback.body ) {
			Entity *ent=whois(callback.body);
			if(ent && c_entity.isFunction())
				c_entity(ent,sqrt((callback.m_point-center).Length()));
			if(c_body.isFunction())
				c_body(callback.body,sqrt((callback.m_point-center).Length()));
		}
	}
}
