#include "render.hpp"
#include "camera.hpp"
#include "sdl.hpp"
#include <cstring>
#include <stdlib.h>
#include <vector>
#include "physic.hpp"
#include "level.hpp"
#include "utility.hpp"
#include "lua.hpp"
#include "interface.hpp"
#include "effect.hpp"
using namespace std;
Color scene_mask(0,0,0,0);
bool show_textures=1;
const float tex_scale=1.0f;
void draw_mask() {
	GPU_RectangleFilled(ren,0,0,SW,SH,scene_mask.color());
}
void draw_bgr() {
	if(!background.size())return;
	GPU_Image *image=textures[background];
	float w=image->w;
	float h=image->h;
	if(w/h>(float)SW/SH) {
		GPU_BlitScale(image,0,ren,SW/2,SH/2,SH/h,SH/h);
	} else {
		GPU_BlitScale(image,0,ren,SW/2,SH/2,SW/w,SW/w);
	}
}
void fixture_draw(b2Body *body,b2Fixture *fix) {
	float a_rad=body->GetAngle();
	float a_deg=a_rad*(180/3.14);
	GPU_Image *tex=0;
	if(show_textures)
		tex=find_texture(F_DATA(fix,texture));

	if(F_DATA(fix,expand) && tex)
		GPU_SetWrapMode(tex, GPU_WRAP_NONE, GPU_WRAP_NONE);
	else
		GPU_SetWrapMode(tex, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);

	switch(F_DATA(fix,type)) {
	case RECT:
	case SQUARE: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(tex) {
			float w=shape->m_vertices[2].x-shape->m_vertices[0].x;
			float h=shape->m_vertices[2].y-shape->m_vertices[0].y;
			if(F_DATA(fix,expand)) {
				GPU_BlitTransformX(tex,0,ren,
								   drawx(body->GetPosition().x),drawy(body->GetPosition().y),
								   (0.5-shape->m_centroid.x/w)*tex->w,
								   (0.5-shape->m_centroid.y/h)*tex->h,a_deg,
								   zoom*w/tex->w,
								   zoom*h/tex->h);
			} else {
				float f[16];
				for(int q=0; q<shape->m_count; q++) {
					b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
					f[q*4]=drawx(body->GetPosition().x+rotatex(v,a_rad));
					f[q*4+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
					f[q*4+2]=(shape->m_vertices[q]-shape->m_vertices[0]).x*(zoom/tex->w);
					f[q*4+3]=(shape->m_vertices[q]-shape->m_vertices[0]).y*(zoom/tex->h);
				}
				short unsigned int index[]= {0,1,3,2};
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
			if(F_DATA(fix,expand)) {
				GPU_BlitTransformX(tex,0,ren,
								   drawx(body->GetPosition().x),drawy(body->GetPosition().y),
								   (0.5+shape->m_p.x/shape->m_radius/2.0)*tex->w,
								   (0.5+shape->m_p.y/shape->m_radius/2.0)*tex->h,a_deg+180,
								   zoom*shape->m_radius*2/tex->w,
								   zoom*shape->m_radius*2/tex->h);
			} else {
				float f[(CIRCLE_QUALITY+2)*4];
				int q;
				for(q=0; q<CIRCLE_QUALITY+1; q++) {
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
		if(tex) {
			b2Vec2 maxv(-100000,-100000),minv(100000,100000);
			for(int q=0; q<shape->b_count; q++) {
				b2Vec2 v(shape->big_polygon[q]);
				maxv.x=max(v.x,maxv.x);
				maxv.y=max(v.y,maxv.y);
				minv.x=min(v.x,minv.x);
				minv.y=min(v.y,minv.y);
			}
			float f[12];
			for(int q=0; q<3; q++) {
				b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
				f[q*4]=drawx(body->GetPosition().x+rotatex(v,a_rad));
				f[q*4+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
				f[q*4+2]=(v-minv).x/(F_DATA(fix,expand)?(maxv-minv).x:(tex->w/100.0f/tex_scale));
				f[q*4+3]=(v-minv).y/(F_DATA(fix,expand)?(maxv-minv).y:(tex->h/100.0f/tex_scale));
			}
			GPU_TriangleBatch(tex,ren,3,f,3,0,GPU_BATCH_XY_ST);
		} else {
			float f[6];
			for(int q=0; q<3; q++) {
				b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
				f[q*2]=drawx(body->GetPosition().x+rotatex(v,a_rad));
				f[q*2+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
			}
			GPU_Polygon(ren,3,f, {255,255,255,255});
		}
	}break;
	case COVER: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(tex) {
			float f[16];
			for(int q=0; q<4; q++) {
				b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
				f[q*4]=drawx(body->GetPosition().x+rotatex(v,a_rad));
				f[q*4+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
				f[q*4+2]=shape->big_polygon[q].x/(F_DATA(fix,expand)?shape->big_polygon[4].x:(tex->w/100.0f/tex_scale));
				if(F_DATA(fix,expand))
					f[q*4+3]=(shape->big_polygon[q].y!=0);
				else
					f[q*4+3]=shape->big_polygon[q].y/(tex->h/100.0f/tex_scale);
			}
			if(F_DATA(fix,expand))
				GPU_SetWrapMode(tex, GPU_WRAP_NONE, GPU_WRAP_NONE);
			else
				GPU_SetWrapMode(tex, GPU_WRAP_REPEAT, GPU_WRAP_NONE);

			short unsigned int index[]= {0,1,3,2};
			GPU_PrimitiveBatch(tex,ren,GPU_TRIANGLE_STRIP,4,f,4,index,GPU_BATCH_XY_ST);
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
	}break;
	}
}
void draw_bodies(uint8_t pos) {
	for(auto &body : bodies) {
		for(b2Fixture *fix=body.second->GetFixtureList(); fix; fix=fix->GetNext()) {
			if(F_DATA(fix,pos)==pos)fixture_draw(body.second,fix);
		}
	}
}
void draw_entities(uint8_t pos) {
	for(auto &en : entities) {
		for(auto &body : en.second->bodies) {
			for(b2Fixture *fix=body.second->GetFixtureList(); fix; fix=fix->GetNext()) {
				if(F_DATA(fix,pos)==pos)fixture_draw(body.second,fix);
			}
		}
		if(pos==3 && en.second->weapon!="" && show_textures) {
			float size_y=1.0f;
			if(en.second->weapon_angle>0.5*M_PI&&en.second->weapon_angle<1.5*M_PI)
				size_y=-1.0f;
			GPU_BlitTransformX(textures["weapon/"+en.second->weapon+".png"],0,ren,
							   drawx(en.second->getx()+en.second->weapon_x),
							   drawy(en.second->gety()+en.second->weapon_y),
							   weapons[en.second->weapon].dx*100,weapons[en.second->weapon].dy*100,
							   en.second->weapon_angle/M_PI*180,zoom/100,size_y*zoom/100);
		}
	}
}
void draw_effects() {
	for(auto &e : effect::effects) {
		int frame=min((int)e.effect->anim.size()-1,int(float(lua::get_time()-e.begin_time)/e.effect->period));
		GPU_BlitScale(e.effect->anim[frame],0,ren,drawx(e.x),drawy(e.y),zoom/100,zoom/100);
	}
}
void draw() {
	GPU_Clear(ren);
	draw_bgr();
	for(int q=0; q<6; q++) {
		draw_bodies(q);
		draw_entities(q);
	}
	draw_effects();
	draw_mask();
	interface.show();
	GPU_Flip(ren);
}
