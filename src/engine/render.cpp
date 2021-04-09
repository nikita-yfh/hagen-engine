#include "render.hpp"
#include "camera.hpp"
#include "sdl.hpp"
#include <cstring>
#include <stdlib.h>
#include <vector>
#include "physic.hpp"
#include "level.hpp"
#include "utility.hpp"
#include "console.hpp"
#include "player.hpp"
#define CIRCLE_QUALITY 50
using namespace std;
Color scene_mask(0,0,0,0);
bool show_textures=1;
void draw_mask() {
	GPU_RectangleFilled(ren,0,0,SW,SH,scene_mask.color());
}
void draw_bgr() {
	if(!background)return;
	float w=background->w;
	float h=background->h;
	if(w/h>(float)SW/SH) {
		GPU_BlitScale(background,0,ren,SW/2,SH/2,w/h,w/h);
	} else {
		GPU_BlitScale(background,0,ren,SW/2,SH/2,h/w,h/w);
	}
}
void fixture_draw(b2Body *body,b2Fixture *fix) {
	float a_rad=body->GetAngle();
	float a_deg=a_rad*(180/3.14);
	GPU_Image *tex=0;
	if(show_textures)
		tex=find_texture(F_DATA(fix,texture));
	switch(F_DATA(fix,type)) {
	case RECT:
	case SQUARE: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(tex) {
			float w=shape->m_vertices[2].x-shape->m_vertices[0].x;
			float h=shape->m_vertices[2].y-shape->m_vertices[0].y;
			if(F_DATA(fix,expand)){
				GPU_BlitTransformX(tex,0,ren,
				   drawx(body->GetPosition().x),drawy(body->GetPosition().y),
				   (0.5-shape->m_centroid.x/w)*tex->w,
				   (0.5-shape->m_centroid.y/h)*tex->h,a_deg,
				   zoom*w/tex->w,
				   zoom*h/tex->h);
			}else{
				float f[16];
				for(int q=0;q<shape->m_count;q++){
					b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
					f[q*4]=drawx(body->GetPosition().x+rotatex(v,a_rad));
					f[q*4+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
					f[q*4+2]=(shape->m_vertices[q]-shape->m_vertices[0]).x*(zoom/tex->w);
					f[q*4+3]=(shape->m_vertices[q]-shape->m_vertices[0]).y*(zoom/tex->h);
				}
				GPU_SetWrapMode(tex, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
				short unsigned int index[]={0,1,3,2};
				GPU_PrimitiveBatch(tex,ren,GPU_TRIANGLE_STRIP,shape->m_count,f,shape->m_count,index,GPU_BATCH_XY_ST);
			}
		} else {
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
			if(F_DATA(fix,expand)){
				GPU_BlitTransformX(tex,0,ren,
				   drawx(body->GetPosition().x),drawy(body->GetPosition().y),
				   (0.5+shape->m_p.x/shape->m_radius/2.0)*tex->w,
				   (0.5+shape->m_p.y/shape->m_radius/2.0)*tex->h,a_deg+180,
				   zoom*shape->m_radius*2/tex->w,
				   zoom*shape->m_radius*2/tex->h);
			}else{
				float f[(CIRCLE_QUALITY+2)*4];
				int q;
				for(q=0;q<CIRCLE_QUALITY+1;q++){
					b2Vec2 v;
					if(!q)v.Set(shape->m_p.x,shape->m_p.y);
					else v.Set(shape->m_p.x+cos(2*M_PI/(CIRCLE_QUALITY)*q)*shape->m_radius,
								shape->m_p.y+sin(2*M_PI/(CIRCLE_QUALITY)*q)*shape->m_radius);
					f[q*4+0]=drawx(body->GetPosition().x+rotatex(v,a_rad));
					f[q*4+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
					f[q*4+2]=(v.x-shape->m_p.x-shape->m_radius)*(zoom/tex->w);
					f[q*4+3]=(v.y-shape->m_p.y-shape->m_radius)*(zoom/tex->h);
				}
				f[q*4+0]=f[4];
				f[q*4+1]=f[5];
				f[q*4+2]=f[6];
				f[q*4+3]=f[7];
				GPU_SetWrapMode(tex, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
				GPU_PrimitiveBatch(tex,ren,GPU_TRIANGLE_FAN,CIRCLE_QUALITY+2,f,CIRCLE_QUALITY+2,0,GPU_BATCH_XY_ST);
			}
		} else {
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
			b2Vec2 maxv(0,0),minv(100000,100000);
			for(int q=0;q<shape->b_count;q++){
				b2Vec2 v(shape->big_polygon[q]);
				maxv.x=max(v.x,maxv.x);
				maxv.y=max(v.y,maxv.y);
				minv.x=min(v.x,minv.x);
				minv.y=min(v.y,minv.y);
			}
			float f[shape->m_count*4];
			for(int q=0;q<shape->m_count;q++){
				b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
				f[q*4]=drawx(body->GetPosition().x+rotatex(v,a_rad));
				f[q*4+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
				f[q*4+2]=(v-minv).x/(F_DATA(fix,expand)?(maxv-minv).x:(tex->w/100));
				f[q*4+3]=(v-minv).y/(F_DATA(fix,expand)?(maxv-minv).y:(tex->h/100));
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
void draw_bodies(uint8_t pos){
	for(auto &body : bodies) {
		for(b2Fixture *fix=body.second->GetFixtureList(); fix; fix=fix->GetNext()) {
			if(F_DATA(fix,pos)==pos)fixture_draw(body.second,fix);
		}
	}
}
void draw_entities(uint8_t pos){
	for(auto &en : entities){
		for(auto &body : en.second->bodies){
			for(b2Fixture *fix=body.second->GetFixtureList(); fix; fix=fix->GetNext()) {
				if(F_DATA(fix,pos)==pos)fixture_draw(body.second,fix);
			}
		}
		if(pos==3 && en.second->weapon!="" && show_textures){
			float size_y=1.0f;
			if(en.second->weapon_angle>0.5*M_PI&&en.second->weapon_angle<1.5*M_PI)
				size_y=-1.0f;
			GPU_BlitTransformX(textures[en.second->weapon+".png"],0,ren,
				drawx(en.second->getx()+en.second->weapon_x),
				drawy(en.second->gety()+en.second->weapon_y),
				weapons[en.second->weapon].dx*100,weapons[en.second->weapon].dy*100,
				en.second->weapon_angle/M_PI*180,zoom/100,size_y*zoom/100);
		}
	}
}
void draw_interface(){
	for(uint8_t q=0;q<player.max_lives;q++){
		GPU_Image *tex=textures[(player.lives>q)?"live2.png":"live1.png"];
		GPU_Blit(tex,0,ren,40+tex->w*q,40);
	}
}
void draw() {
	GPU_Clear(ren);
	draw_bgr();
	for(int q=0;q<5;q++){
		draw_bodies(q);
		draw_entities(q);
	}
	draw_mask();
	draw_interface();
	console.show();
	GPU_Flip(ren);
}
