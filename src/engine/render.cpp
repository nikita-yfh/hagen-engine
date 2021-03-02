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
	float a_rad=body->GetAngle();
	float a_deg=a_rad*(180/3.14);
	std::string h=F_DATA(fix,id);
    GPU_Image *tex=find_texture(F_DATA(fix,texture));
	switch(F_DATA(fix,type)) {
	case RECT:
	case SQUARE: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(tex) {
			GPU_BlitTransformX(tex,0,ren,
                drawx(body->GetPosition().x),drawy(body->GetPosition().y),
                (shape->m_centroid.x+0.5)*tex->w,
                (shape->m_centroid.y+0.5)*tex->h,a_deg,
                zoom*(shape->m_vertices[0].x-shape->m_vertices[2].x)/tex->w,
                zoom*(shape->m_vertices[0].y-shape->m_vertices[2].y)/tex->h);
		} else {
			float x[4];
			float y[4];
			for(int q=0; q<4; q++) {
				float xp=shape->m_vertices[q].x;
				float yp=shape->m_vertices[q].y;
				x[q]=drawx(body->GetPosition().x+xp*cos(a_rad)-yp*sin(a_rad));
				y[q]=drawy(body->GetPosition().y+yp*cos(a_rad)+xp*sin(a_rad));
			}
			for(int q=0;q<4;q++)
                GPU_Line(ren,x[q],y[q],x[(q+1)%4],y[(q+1)%4],{255,255,255,255});
		}
	}
	break;
	case CIRCLE: {
		b2CircleShape *shape=(b2CircleShape*)fix->GetShape();
		if(tex) {
			GPU_BlitTransformX(tex,0,ren,
                drawx(body->GetPosition().x),drawy(body->GetPosition().y),
                (-shape->m_p.x+0.5)*tex->w,
                (-shape->m_p.y+0.5)*tex->h,a_deg,
                zoom*shape->m_radius*2/tex->w,
                zoom*shape->m_radius*2/tex->h);
		} else {
            float x=drawx(body->GetPosition().x+shape->m_p.x*cos(a_rad)-shape->m_p.y*sin(a_rad));
            float y=drawy(body->GetPosition().y+shape->m_p.y*cos(a_rad)+shape->m_p.x*sin(a_rad));
            float xp=x+(zoom*shape->m_radius)*cos(a_rad);
            float yp=y+(zoom*shape->m_radius)*sin(a_rad);
            GPU_Circle(ren,x,y,shape->m_radius*zoom,{255,255,255,255});
            GPU_Line(ren,x,y,xp,yp,{255,255,255,255});
		}
	}
	break;
	/*case LINE: {
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