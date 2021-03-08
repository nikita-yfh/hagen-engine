#include "game.hpp"
#include "box2d.h"
#include "SDL.h"
#include "level.hpp"
#include "physic.hpp"
#include "render.hpp"
#include "lua.hpp"
#include "camera.hpp"
#include "sdl.hpp"
#include "utility.hpp"
#define TYPE(a,b) (static_cast<a>(b))
#define DEBUG
bool game() {
	bool run=1;
	int fps=SDL_GetTicks();
	bool jump=0;
	while(run) {
		bool connected=0;
		for(b2ContactEdge *e=get_entity("player").get_body("wheel")->GetContactList();e;e=e->next){
			b2Body *b1=e->contact->GetFixtureA()->GetBody();
			b2Body *b2=e->contact->GetFixtureB()->GetBody();
			if(B_DATA(b1,id)!="body"&&B_DATA(b2,id)!="body")connected=1;
		}
		while(SDL_PollEvent(&e)){
			switch(e.type) {
			case SDL_QUIT:
				run=0;
				break;
#ifdef DEBUG
			static bool press=0;
			case SDL_MOUSEBUTTONDOWN:
				if(e.button.button==SDL_BUTTON_RIGHT){
					press=1;
				}
			case SDL_MOUSEMOTION:
				if(press){
					if(abs(e.motion.xrel)<100){
						cx+=e.motion.xrel*3;
						cy+=e.motion.yrel*3;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				press=0;
				break;
#endif
			case SDL_KEYDOWN:
				if(e.key.keysym.sym==SDLK_SPACE)jump=0;
				break;
			case SDL_KEYUP:
				if(e.key.keysym.sym==SDLK_SPACE)jump=1;
				break;
			}
			if(e.type==SDL_KEYUP && jump && connected){
				jump=0;
				get_entity("player").get_body("wheel")->CenterImpulse(0,-2000);
				printf("jump\n");
			}
		}
		b2Joint *j=get_entity("player").get_joint("joint");
		b2Body *b=get_entity("player").get_body("body");
		if(key[SDL_SCANCODE_A]){
			j->EnableMotor(1);
			j->SetMotorSpeed(-30);
			j->SetMaxMotorTorque(1000);
			if(b->GetLinearVelocityX()>-5)b->ApplyForceToCenter({-10000,0},1);
		}else if(key[SDL_SCANCODE_D]){
			j->EnableMotor(1);
			j->SetMotorSpeed(30);
			j->SetMaxMotorTorque(1000);
			if(b->GetLinearVelocityX()<5)b->ApplyForceToCenter({10000,0},1);
		}else{
			j->EnableMotor(0);
		}
		center_body(b);
		draw();
		SDL_Delay(1);
		lua_gameloop();
		int step=SDL_GetTicks()-fps;
		fps=SDL_GetTicks();
		world.Step(step/1000.0f,10,10);
	}
	return 0;
}
