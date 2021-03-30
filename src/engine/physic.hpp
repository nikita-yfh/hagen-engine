#pragma once
#include <string>
#include "box2d.h"
#include "entity.hpp"
#define TSTAT          0
#define TKNM           1
#define TDYN           2

#define TBGR           0
#define TPHS1          1
#define TPHS2          2
#define TPHS3          3
#define TFGR           4
#define TNONE          5

#define POLYGON        1
#define RECT           2
#define SQUARE         3
#define CIRCLE         4
#define LINE           5

#define ST_DENDITY     1.0f
#define ST_FRICTION    0.3f
#define ST_RESTITUTION 0.0f

#define B_DATA(a,n) (a->GetUserData()->n)
#define BD_DATA(a,n) (a.userData->n)
#define J_DATA(a,n) (a->GetUserData()->n)
#define JD_DATA(a,n) (a.userData->n)
#define F_DATA(a,n) (a->GetUserData()->n)
#define FD_DATA(a,n) (a.userData->n)
b2Body *create_body(string type,string id,float x,float y);
Entity *create_entity(string type,string id,float x,float y);
void set_gravity(float x,float y);
void destroy_body(string id);
void destroy_joint(string id);
void destroy_entity(string id);
bool collide(b2Body *b1,b2Body *b2);
bool entity_collide(Entity *ent,b2Body *b);
