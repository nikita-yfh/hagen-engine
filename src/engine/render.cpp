#include "render.hpp"
#include "SDL2_gfxPrimitives.h"
#include "camera.hpp"
#include "sdl.hpp"
#include <cstring>
#include <stdlib.h>
#include <vector>
#include "physic.hpp"
#include "level.hpp"
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
	GPU_RectangleFilled(ren,0,0,SW,SH,{scene_mask.r,scene_mask.g,
				     scene_mask.b,scene_mask.a});
}
void draw_bgr() {
	if(!background)return;
	float w=background->w;
	float h=background->h;
	if(w/h>(float)SW/SH) {
		GPU_BlitScale(background,0,ren,int(SW-w/h*SH)/2,0,int(w/h),1);
	} else {
		GPU_BlitScale(background,0,ren,0,int(SH-h/w*SW)/2,1,int(h/w));
	}
}
void fixture_draw(b2Body *body,b2Fixture *fix) {
	float a=body->GetAngle()* (180/3.14);
	std::string h=F_DATA(fix,id);
	switch(F_DATA(fix,type)) {
	case RECT:
	case SQUARE: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(find_texture(F_DATA(fix,texture))) {
			/*SDL_Rect r= {
				,

				int(zoom*(shape->m_vertices[2].x-shape->m_vertices[0].x)),
				int(zoom*(shape->m_vertices[2].y-shape->m_vertices[0].y))
			};
			SDL_Point p= {
				-int(),
					-int(
				};*/
			//SDL_RenderCopyEx(ren,,0,&r,a*(180.0f/M_PI),&p,SDL_RendererFlip::SDL_FLIP_NONE);
			GPU_Image *tex=find_texture(F_DATA(fix,texture));
			//int x=(shape->m_vertices[2].x+shape->m_vertices[0].x)/2;
			//int y=(shape->m_vertices[2].y+shape->m_vertices[0].y)/2;
			GPU_BlitTransformX(tex,0,ren,
                zoom*(body->GetPosition().x+shape->m_centroid.x),
                zoom*(body->GetPosition().y+shape->m_centroid.y),
                zoom*(1),
                zoom*(1),
                a,
                zoom*(shape->m_vertices[2].x-shape->m_vertices[0].x)/tex->w,
                zoom*(shape->m_vertices[2].y-shape->m_vertices[0].y)/tex->h);
		} else {
			short *x=new short[4];
			short *y=new short[4];
			for(int q=0; q<4; q++) {
				float xp=shape->m_vertices[q].x;
				float yp=shape->m_vertices[q].y;
				x[q]=drawix(body->GetPosition().x+xp*cos(a)-yp*sin(a));
				y[q]=drawiy(body->GetPosition().y+yp*cos(a)+xp*sin(a));
			}
			//polygonColor(ren,x,y,4,0xFFFFFFFF);
			//GPU_
			delete[]x;
			delete[]y;
		}
	}
	break;
	/*case CIRCLE: {
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
	break;*/
	}
}
void body_draw(b2Body *body) {
	for(b2Fixture *fix=body->GetFixtureList(); fix; fix=fix->GetNext()) {
		fixture_draw(body,fix);
	}
}
void draw() {
    GPU_Clear(ren);
	draw_bgr();
	for(unsigned q=0; q<bodies.size(); q++) {
		body_draw(bodies[q]);
	}
	draw_mask();
	GPU_Flip(ren);
}
