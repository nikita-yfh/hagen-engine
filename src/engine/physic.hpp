#pragma once
#include <string>
#include "box2d.h"
#define TSTAT          0
#define TKNM           1
#define TDYN           2

#define TBGR           0
#define TMGR           1
#define TFGR           2

#define MOVE           0
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

struct contactListener : b2ContactListener {
	void BeginContact(b2Contact* contact);
};
