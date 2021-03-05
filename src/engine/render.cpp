#include "render.hpp"
//#include "SDL2_gfxPrimitives.h"
#include "camera.hpp"
#include "sdl.hpp"
#include <cstring>
#include <stdlib.h>
#include <vector>
#include "physic.hpp"
#include "level.hpp"
#include "utility.hpp"
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
	GPU_RectangleFilled(ren,0,0,SW,SH, {scene_mask.r,scene_mask.g,
										scene_mask.b,scene_mask.a
									   });
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
	static float f=1;
	if(key[SDL_SCANCODE_1])f+=0.01;
	if(key[SDL_SCANCODE_2])f-=0.01;
	printf("%g\n",f);
	switch(F_DATA(fix,type)) {
	case RECT:
	case SQUARE: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(tex) {
			GPU_BlitTransformX(tex,0,ren,
							   drawx(body->GetPosition().x),drawy(body->GetPosition().y),
							   (-shape->m_centroid.x+0.5)*tex->w/f,
							   (-shape->m_centroid.y+0.5)*tex->h/f,a_deg,
							   zoom*(shape->m_vertices[2].x-shape->m_vertices[0].x)/tex->w,
							   zoom*(shape->m_vertices[2].y-shape->m_vertices[0].y)/tex->h);
		//} else {
			float x[4];
			float y[4];
			for(int q=0; q<4; q++) {
				x[q]=drawx(body->GetPosition().x+rotatex(shape->m_vertices[q],a_rad));
				y[q]=drawy(body->GetPosition().y+rotatey(shape->m_vertices[q],a_rad));
			}
			for(int q=0; q<4; q++)
				GPU_Line(ren,x[q],y[q],x[(q+1)%4],y[(q+1)%4], {255,255,255,255});
		}
	}
	break;
	case CIRCLE: {
		b2CircleShape *shape=(b2CircleShape*)fix->GetShape();
		if(tex) {
			GPU_BlitTransformX(tex,0,ren,
							   drawx(body->GetPosition().x),drawy(body->GetPosition().y),
							   (-shape->m_p.x+0.5)*tex->w,
							   (-shape->m_p.x+0.5)*tex->h,a_deg+180,
							   zoom*shape->m_radius*2/tex->w,
							   zoom*shape->m_radius*2/tex->h);
		} /*else*/ {
			float x=drawx(body->GetPosition().x+rotatex(shape->m_p,a_rad));
			float y=drawy(body->GetPosition().y+rotatey(shape->m_p,a_rad));
			float xp=x+(zoom*shape->m_radius)*cos(a_rad);
			float yp=y+(zoom*shape->m_radius)*sin(a_rad);
			GPU_Circle(ren,x,y,shape->m_radius*zoom, {255,255,255,255});
			GPU_Line(ren,x,y,xp,yp, {255,255,255,255});
		}
	}
	break;
	case LINE: {
		b2EdgeShape *shape=(b2EdgeShape*)fix->GetShape();
		float x1=drawix(body->GetPosition().x+rotatex(shape->m_vertex1,a_rad));
		float y1=drawiy(body->GetPosition().y+rotatey(shape->m_vertex1,a_rad));
		float x2=drawix(body->GetPosition().x+rotatex(shape->m_vertex2,a_rad));
		float y2=drawiy(body->GetPosition().y+rotatey(shape->m_vertex2,a_rad));
		GPU_Line(ren,x1,y1,x2,y2, {255,255,255,255});
	}
	break;
	case POLYGON: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(tex){
			float f[shape->m_count*4];
			b2Vec2 maxv(0,0),minv(100000,100000);
			for(int q=0;q<shape->m_count;q++){
				b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
				f[q*4]=drawx(body->GetPosition().x+rotatex(v,a_rad));
				f[q*4+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
			}
			for(int q=0;q<shape->b_count;q++){
				b2Vec2 v(shape->big_polygon[q]);
				maxv.x=max(v.x,maxv.x);
				maxv.y=max(v.y,maxv.x);
				minv.x=min(v.x,minv.x);
				minv.y=min(v.y,minv.x);
			}
			for(int q=0;q<shape->m_count;q++){
				f[q*4+2]=(shape->m_vertices[q]-minv).x*(zoom/tex->w);
				f[q*4+3]=(shape->m_vertices[q]-minv).y*(zoom/tex->h);
			}
			GPU_SetWrapMode(tex, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
			GPU_TriangleBatch(tex,ren,shape->m_count,f,shape->m_count,0,GPU_BATCH_XY_ST);
		}else{
			float f[shape->m_count*2];
			for(int q=0;q<shape->m_count;q++){
				b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
				f[q*2]=drawx(body->GetPosition().x+rotatex(v,a_rad));
				f[q*2+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
			}
			GPU_Polygon(ren,shape->m_count,f,{255,255,255,255});
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
	GPU_Clear(ren);
	draw_bgr();
	for(unsigned q=0; q<bodies.size(); q++) {
		body_draw(bodies[q]);
	}
	draw_mask();
	GPU_Flip(ren);
}
