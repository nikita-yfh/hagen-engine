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
		for(b2Contact *contact=world.GetContactList();contact;contact=contact->GetNext()){
			b2Body *b1=contact->GetFixtureA()->GetBody();
			b2Body *b2=contact->GetFixtureB()->GetBody();
			Entity *en1=0,*en2=0;
			for(Entity &en : entities){
				for(b2Body *b : en.bodies){
					if(b==b1)en1=&en;
					else if(b==b2)en2=&en;
				}
			}
			if(en1!=en2&&(en1||en2)){
				connected=1;
				break;
			}
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
				if(e.key.keysym.sym==SDLK_SPACE)jump=1;
				break;
			case SDL_KEYUP:
				if(e.key.keysym.sym==SDLK_SPACE)jump=0;
				break;
			}
			if(e.type==SDL_KEYDOWN && jump && connected){
				jump=0;
				get_entity("player").get_body("wheel")->CenterImpulse(0,-2000);
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
