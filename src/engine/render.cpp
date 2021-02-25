#include "render.hpp"
#include "camera.hpp"
#include "sdl.hpp"
using namespace std;
Color scene_mask(0,0,0,0);
void Color::set(int r,int g,int b,int a) {
	this->r=r;
	this->g=g;
	this->b=b;
	this->a=a;
}
Color::Color(int r,int g,int b,int a) {
	set(r,g,b,a);
}
Color::Color(int r,int g,int b) {
	set(r,g,b,255);
}
SDL_Color Color::color() {
	return SDL_Color({r,g,b,a});
}
void draw_mask() {
	SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(ren, scene_mask.r,scene_mask.g,
				     scene_mask.b,scene_mask.a);
	SDL_RenderFillRect(ren,0);
	SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_NONE);
}
void draw_bgr() {
	if(!background)return;
	int w,h,ww,hw;
	SDL_GetWindowSize(window,&ww,&hw);
	SDL_QueryTexture(background,0,0,&w,&h);
	if((float)w/h>(float)ww/hw) {
		SDL_Rect r= {int(ww-(float)w/(float)h*hw)/2,0,int((float)w/(float)h*hw),hw};
		SDL_RenderCopy(ren,background,0,&r);
	} else {
		SDL_Rect r= {0,int(hw-(float)h/(float)w*ww)/2,ww,int((float)h/(float)w*ww)};
		SDL_RenderCopy(ren,background,0,&r);
	}
}
void fixture_draw(b2Body *body,b2Fixture *fix) {
	float a=body->GetAngle();
	std::string h=F_DATA(fix,id);
	switch(F_DATA(fix,type)) {
	case RECT:
	case SQUARE: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(find_texture(F_DATA(fix,texture))) {
			SDL_Rect r= {
				drawix(body->GetPosition().x+shape->m_vertices[0].x),
				drawiy(body->GetPosition().y+shape->m_vertices[0].y),
				int(zoom*(shape->m_vertices[2].x-shape->m_vertices[0].x)),
				int(zoom*(shape->m_vertices[2].y-shape->m_vertices[0].y))
			};
			SDL_Point p= {
				-int(zoom*(shape->m_vertices[0].x)),
					-int(zoom*(shape->m_vertices[0].y))
				};
			SDL_RenderCopyEx(ren,find_texture(F_DATA(fix,texture)),0,&r,a*(180.0f/M_PI),&p,SDL_RendererFlip::SDL_FLIP_NONE);
		} else {
			short *x=new short[4];
			short *y=new short[4];
			for(int q=0; q<4; q++) {
				float xp=shape->m_vertices[q].x;
				float yp=shape->m_vertices[q].y;
				x[q]=drawix(body->GetPosition().x+xp*cos(a)-yp*sin(a));
				y[q]=drawiy(body->GetPosition().y+yp*cos(a)+xp*sin(a));
			}
			polygonColor(ren,x,y,4,0xFFFFFFFF);
			delete[]x;
			delete[]y;
		}
	}
	break;
	case CIRCLE: {
		b2CircleShape *shape=(b2CircleShape*)fix->GetShape();
		short x=drawx(body->GetPosition().x+shape->m_p.x*cos(a)-shape->m_p.y*sin(a));
		short y=drawy(body->GetPosition().y+shape->m_p.y*cos(a)+shape->m_p.x*sin(a));
		short xp=x+(zoom*shape->m_radius)*cos(a);
		short yp=y+(zoom*shape->m_radius)*sin(a);
		if(find_texture(F_DATA(fix,texture))) {
			SDL_Rect r= {
				drawix(body->GetPosition().x+shape->m_p.x-shape->m_radius),
				drawiy(body->GetPosition().y+shape->m_p.y-shape->m_radius),
				int(zoom*shape->m_radius*2),
				int(zoom*shape->m_radius*2)
			};
			SDL_Point p= {
				-int(zoom*(shape->m_p.x-shape->m_radius)),
					-int(zoom*(shape->m_p.y-shape->m_radius))
				};
			SDL_RenderCopyEx(ren,find_texture(F_DATA(fix,texture)),0,&r,a*(180.0f/M_PI),&p,SDL_RendererFlip::SDL_FLIP_NONE);
		} else {
			circleColor(ren,x,y,zoom*shape->m_radius,0xFFFFFFFF);
			lineColor(ren,x,y,xp,yp,0xFFFFFFFF);
		}
	}
	break;
	case LINE: {
		b2EdgeShape *shape=(b2EdgeShape*)fix->GetShape();
		short x1=drawix(body->GetPosition().x+shape->m_vertex1.x*cos(a)-shape->m_vertex1.y*sin(a));
		short y1=drawiy(body->GetPosition().y+shape->m_vertex1.y*cos(a)+shape->m_vertex1.x*sin(a));
		short x2=drawix(body->GetPosition().x+shape->m_vertex2.x*cos(a)-shape->m_vertex2.y*sin(a));
		short y2=drawiy(body->GetPosition().y+shape->m_vertex2.y*cos(a)+shape->m_vertex2.x*sin(a));
		lineColor(ren,x1,y1,x2,y2,0xFFFFFFFF);
	}
	break;
	case POLYGON: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(shape->cache && F_DATA(fix,texture)!="") {
			float minx=1000000000,miny=1000000000,maxx=0,maxy=0;
			for(int q=0; q<shape->b_count; q++) {
				minx=std::min(minx,shape->big_polygon[q].x);
				miny=std::min(miny,shape->big_polygon[q].y);
				maxx=std::max(maxx,shape->big_polygon[q].x);
				maxy=std::max(maxy,shape->big_polygon[q].y);
			}
			SDL_Rect r= {
				drawix(body->GetPosition().x+minx),
				drawiy(body->GetPosition().y+miny),
				int(zoom*(maxx-minx)),
				int(zoom*(maxy-miny))
			};
			SDL_Point p= {
				-int(zoom*minx),
					-int(zoom*miny)
				};
			SDL_RenderCopyEx(ren,shape->cache,0,&r,a*(180.0f/M_PI),&p,SDL_RendererFlip::SDL_FLIP_NONE);
		} else {
			short *xp=new short[shape->m_count];
			short *yp=new short[shape->m_count];
			for(int q=0; q<shape->m_count; q++) {
				float x=shape->m_vertices[q].x;
				float y=shape->m_vertices[q].y;
				xp[q]=drawix(body->GetPosition().x+x*cos(a)-y*sin(a));
				yp[q]=drawiy(body->GetPosition().y+y*cos(a)+x*sin(a));
			}
			polygonColor(ren,xp,yp,shape->m_count,0xFFFFFFFF);
			delete[]xp;
			delete[]yp;
		}
	}
	break;
	}
}
void body_draw(b2Body *body) {
	for(b2Fixture *fix=body->GetFixtureList(); fix; fix=fix->GetNext()) {
		fixture_draw(body,fix);
	}
}
void draw() {
	SDL_SetRenderDrawColor(ren,0,0,0,0);
	SDL_RenderClear(ren);
	draw_bgr();
	for(unsigned q=0; q<bodies.size(); q++) {
		body_draw(bodies[q]);
	}
	draw_mask();
	SDL_RenderPresent(ren);
}
