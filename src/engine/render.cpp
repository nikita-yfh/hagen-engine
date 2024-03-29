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
#include "shader.hpp"
#include "text.hpp"
using namespace std;
float tex_scale=1.0f;
float weapon_scale=1.0f;
float effect_scale=1.0f;
#ifdef DEBUG
const bool debug=true;
#else
const bool debug=false;
#endif
GPU_Image *img1;
GPU_Target *ren1;
void init_target() {
	img1=GPU_CreateImage(SW,SH,GPU_FORMAT_RGBA);
	ren1=GPU_LoadTarget(img1);
}
void delete_target() {
	GPU_FreeImage(img1);
	GPU_FreeTarget(ren1);
}
static void draw_bgr() {
	if(!background.size())return;
	enable_shader(background);
	GPU_Image *image=textures[background];
	float w=image->w;
	float h=image->h;
	if(w/h>(float)SW/SH) {
		GPU_BlitScale(image,0,ren1,SW/2,SH/2,SH/h,SH/h);
	} else {
		GPU_BlitScale(image,0,ren1,SW/2,SH/2,SW/w,SW/w);
	}
	disable_shaders();
}
static inline bool in_range(float val,float a,float b){
	return val>a && val<b;
}
static void fixture_draw(b2Body *body,b2Fixture *fix) {
	if(!on_screen_f(fix))return;
	float a_rad=body->GetAngle();
	float a_deg=deg(a_rad);
	GPU_Image *tex=0;
	tex=find_texture(F_DATA(fix,texture));

	if(F_DATA(fix,expand) && tex)
		GPU_SetWrapMode(tex, GPU_WRAP_NONE, GPU_WRAP_NONE);
	else
		GPU_SetWrapMode(tex, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);

	if(tex)
		enable_shader(F_DATA(fix,texture));
	switch(F_DATA(fix,type)) {
	case RECT:
	case SQUARE: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(tex) {
			float w=(shape->m_vertices[1]-shape->m_vertices[0]).Length();
			float h=(shape->m_vertices[3]-shape->m_vertices[0]).Length();
			if(F_DATA(fix,expand)) {
				b2Vec2 v=body->GetPosition()+rotate(shape->m_centroid,a_rad);
				GPU_BlitTransform(tex,0,ren1,
					drawx(v.x),drawy(v.y),
					a_deg+deg(F_DATA(fix,angle)),
					zoom*w/tex->w,zoom*h/tex->h);
			} else {
				float f[16];
				for(int q=0; q<shape->m_count; q++) {
					b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
					f[q*4]=drawx(body->GetPosition().x+rotatex(v,a_rad));
					f[q*4+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
					f[q*4+2]=rotatex(shape->m_vertices[q]-shape->m_vertices[0],-F_DATA(fix,angle))*(100.0f/tex->w/tex_scale);
					f[q*4+3]=rotatey(shape->m_vertices[q]-shape->m_vertices[0],-F_DATA(fix,angle))*(100.0f/tex->h/tex_scale);
				}
				short unsigned int index[]= {0,1,3,1,2,3};
				GPU_TriangleBatch(tex,ren1,4,f,6,index,GPU_BATCH_XY_ST);
			}
		} else if(debug){
			float x[4];
			float y[4];
			for(int q=0; q<4; q++) {
				x[q]=drawx(body->GetPosition().x+rotatex(shape->m_vertices[q],a_rad));
				y[q]=drawy(body->GetPosition().y+rotatey(shape->m_vertices[q],a_rad));
			}
			for(int q=0; q<4; q++)
				GPU_Line(ren1,x[q],y[q],x[(q+1)%4],y[(q+1)%4], {255,255,255,255});
		}
	}
	break;
	case CIRCLE: {
		b2CircleShape *shape=(b2CircleShape*)fix->GetShape();
		if(tex) {
			if(F_DATA(fix,expand)) {
				b2Vec2 v=body->GetPosition()+rotate(shape->m_p,a_rad);
				GPU_BlitTransform(tex,0,ren1,
					drawx(v.x),drawy(v.y),
					a_deg+F_DATA(fix,angle)/M_PI*180.0f,
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
					f[q*4+2]=rotatex(v-shape->m_p-b2Vec2(shape->m_radius,shape->m_radius),-F_DATA(fix,angle))*(100.0f/tex->w/tex_scale);
					f[q*4+3]=rotatey(v-shape->m_p-b2Vec2(shape->m_radius,shape->m_radius),-F_DATA(fix,angle))*(100.0f/tex->h/tex_scale);
				}
				f[q*4+0]=f[4];
				f[q*4+1]=f[5];
				f[q*4+2]=f[6];
				f[q*4+3]=f[7];
				short unsigned int index[CIRCLE_QUALITY*3];
				for(int q=0; q<CIRCLE_QUALITY; q++) {
					index[q*3+0]=0;
					index[q*3+1]=q+1;
					index[q*3+2]=q+2;
				}
				GPU_TriangleBatch(tex,ren1,CIRCLE_QUALITY+2,f,CIRCLE_QUALITY*3,index,GPU_BATCH_XY_ST);
			}
		} else  if(debug){
			float x=drawx(body->GetPosition().x+rotatex(shape->m_p,a_rad));
			float y=drawy(body->GetPosition().y+rotatey(shape->m_p,a_rad));
			float xp=x+(zoom*shape->m_radius)*cos(a_rad);
			float yp=y+(zoom*shape->m_radius)*sin(a_rad);
			float r=shape->m_radius*zoom;
			GPU_Circle(ren1,x,y,r, {255,255,255,255});
			GPU_Line(ren1,x,y,xp,yp, {255,255,255,255});
		}
	}
	break;
	case LINE: {
		if(debug){
			b2EdgeShape *shape=(b2EdgeShape*)fix->GetShape();
			float x1=drawix(body->GetPosition().x+rotatex(shape->m_vertex1,a_rad));
			float y1=drawiy(body->GetPosition().y+rotatey(shape->m_vertex1,a_rad));
			float x2=drawix(body->GetPosition().x+rotatex(shape->m_vertex2,a_rad));
			float y2=drawiy(body->GetPosition().y+rotatey(shape->m_vertex2,a_rad));
			GPU_Line(ren1,x1,y1,x2,y2, {255,255,255,255});
		}
	}
	break;
	case POLYGON: {
		int t=SDL_GetTicks();
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
				if(F_DATA(fix,expand)){
					f[q*4+2]=(v-minv).x/(maxv-minv).x;
					f[q*4+3]=(v-minv).y/(maxv-minv).y;
				}else{
					f[q*4+2]=(v-minv).x/(tex->w/100.0*tex_scale);
					f[q*4+3]=(v-minv).y/(tex->h/100.0*tex_scale);
				}
			}
			GPU_TriangleBatch(tex,ren1,3,f,3,0,GPU_BATCH_XY_ST);
		} else if(debug){
			float f[6];
			for(int q=0; q<3; q++) {
				b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
				f[q*2]=drawx(body->GetPosition().x+rotatex(v,a_rad));
				f[q*2+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
			}
			GPU_Polygon(ren1,3,f, {255,255,255,255});
		}
	}
	break;
	case COVER: {
		b2PolygonShape *shape=(b2PolygonShape*)fix->GetShape();
		if(tex) {
			float f[16];
			bool ok=0;
			for(int q=0; q<4; q++) {
				b2Vec2 v(shape->m_vertices[q].x,shape->m_vertices[q].y);
				f[q*4]=drawx(body->GetPosition().x+rotatex(v,a_rad));
				f[q*4+1]=drawy(body->GetPosition().y+rotatey(v,a_rad));
				f[q*4+2]=shape->big_polygon[q].x/(F_DATA(fix,expand)?shape->big_polygon[4].x:(tex->w/100.0f*tex_scale));
				if(F_DATA(fix,expand))
					f[q*4+3]=(shape->big_polygon[q].y!=0);
				else
					f[q*4+3]=shape->big_polygon[q].y/(tex->h/100.0f*tex_scale);
				if(in_range(f[q*4],0,SW) ||
					in_range(f[q*4+1],0,SH))ok=1;
			}
			if(!ok)break;
			if(F_DATA(fix,expand))
				GPU_SetWrapMode(tex, GPU_WRAP_NONE, GPU_WRAP_NONE);
			else
				GPU_SetWrapMode(tex, GPU_WRAP_REPEAT, GPU_WRAP_NONE);

			short unsigned int index[]= {0,1,3,1,2,3};
			GPU_TriangleBatch(tex,ren1,4,f,6,index,GPU_BATCH_XY_ST);
		} else if(debug){
			float x[4];
			float y[4];
			for(int q=0; q<4; q++) {
				x[q]=drawx(body->GetPosition().x+rotatex(shape->m_vertices[q],a_rad));
				y[q]=drawy(body->GetPosition().y+rotatey(shape->m_vertices[q],a_rad));
			}
			for(int q=0; q<4; q++)
				GPU_Line(ren1,x[q],y[q],x[(q+1)%4],y[(q+1)%4], {255,255,255,255});
		}
	}
	break;
	}
	disable_shaders();
}
static void draw_bodies(int8_t pos) {
	for(auto &body : bodies) {
		for(b2Fixture *fix=body.second->GetFixtureList(); fix; fix=fix->GetNext()) {
			if(F_DATA(fix,layer)==pos)fixture_draw(body.second,fix);
		}
	}
}
static void draw_entities(int8_t pos) {
	for(auto &en : entities) {
		for(auto &body : en.second->bodies) {
			for(b2Fixture *fix=body.second->GetFixtureList(); fix; fix=fix->GetNext()) {
				if(F_DATA(fix,layer)==pos)fixture_draw(body.second,fix);
			}
		}
		if(pos==1 && en.second->weapon.texture!="") {
			enable_shader(en.second->weapon.texture);
			float size_y=weapon_scale;
			if(en.second->weapon.angle>0.5*M_PI&&en.second->weapon.angle<1.5*M_PI)
				size_y*=-1;
			GPU_BlitTransformX(textures[en.second->weapon.texture],0,ren1,
							   drawx(en.second->getx()+en.second->weapon.point_x),
							   drawy(en.second->gety()+en.second->weapon.point_y),
							   en.second->weapon.dx*100,en.second->weapon.dy*100,
							   en.second->weapon.angle/M_PI*180,weapon_scale*zoom/100,size_y*zoom/100);
			disable_shaders();
		}
	}
}
static void draw_effects() {
	for(auto &e : effect::effects) {
		int frame=max(0,min((int)e.effect->anim.size()-1,int(float(lua::get_time()-e.begin_time)/e.effect->period)));
		GPU_BlitScale(e.effect->anim[frame],0,ren1,drawx(e.x),drawy(e.y),zoom/100*effect_scale,zoom/100*effect_scale);
	}
}
void draw1() {
	GPU_Clear(ren);
	GPU_Clear(ren1);
	draw_bgr();
	for(int q=-128; q<128; q++) {
		draw_bodies(q);
		draw_entities(q);
	}
	draw_effects();
}
void draw2(){
	enable_shader("_all");
	GPU_Blit(img1,0,ren,SW/2,SH/2);
	disable_shaders();
	text::draw();
	interface.game_interface.draw();
	interface.draw();
	GPU_Flip(ren);
}
