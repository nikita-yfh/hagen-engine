#include "sdl.h"
#include "SDL.h"
#include "level.h"
#include "SDL_image.h"
#include "SDL_FontCache.h"
#include "SDL2_gfxPrimitives.h"
#include "utility.h"
#include "physic.h"
#include "camera.h"
#include <string>
FC_Font *font;
SDL_Event e;
int SW=0;
int SH=0;
SDL_Renderer *ren;
SDL_Window *window;
const uint8_t* key=SDL_GetKeyboardState(0);
using namespace std;
std::vector<Texture>textures;
SDL_Texture *background=0;
void init(const char* title,int w,int h) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	window=SDL_CreateWindow(title,100,100,w,h,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	ren=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	SW=w;
	SH=h;
}
SDL_Texture *find_texture(std::string id){
    for(int q=0;q<textures.size();q++)
        if(textures[q].id==id)return textures[q].texture;
    return 0;
}
void quit() {
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
void panic(string name,string message){
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,name.c_str(),message.c_str(),window);
    quit();
    exit(1);
}

void load_cursor() {
	SDL_Surface *sur=IMG_Load("cursors/default.png");
	SDL_Cursor *cur=SDL_CreateColorCursor(sur,0,0);
	SDL_FreeSurface(sur);
	SDL_SetCursor(cur);
}
void load_textures() {
    for(int q=0;q<bodies.size();q++){
		for(b2Fixture *fix=bodies[q]->GetFixtureList(); fix; fix=fix->GetNext()) {
            string str=F_DATA(fix,texture);
            if(str.size() && !find_texture(str)){
                SDL_Texture *texture=IMG_LoadTexture(ren,("textures/"+str).c_str());
                if(!texture)throw string(SDL_GetError());
                textures.push_back({texture,str});
            }
        }
    }
}
void configure_textures() {
}
void destroy_textures() {
}
void load_background(std::string name) {
    string path="backgrounds/"+name;
	if(!exist_file(path))throw string("Loading \""+name+"\" background failed: file not found");
	if(background)SDL_DestroyTexture(background);
	background=IMG_LoadTexture(ren,path.c_str());
	if(!background)throw string("Loading \""+name+"\" background failed: "+SDL_GetError());
}
void create_cache() {
    SDL_Texture *prev_tex=0;
    b2Vec2 *prev_vec=0;
	for(int q=0; q<bodies.size(); q++) {
		for(b2Fixture *fix=bodies[q]->GetFixtureList(); fix; fix=fix->GetNext()) {
			if(F_DATA(fix,type)==POLYGON) {
				b2PolygonShape *shape=((b2PolygonShape*)(fix->GetShape()));
				if(F_DATA(fix,texture).size()){
                    if(shape->cache) SDL_DestroyTexture(shape->cache);
                    if(prev_tex && prev_vec==shape->big_polygon){
                        shape->cache=prev_tex;
                    }else{
                        float minx=0x80000000,miny=0x80000000,maxx=0,maxy=0;
                        short *xp=new short[shape->b_count];
                        short *yp=new short[shape->b_count];
                        for(int q=0;q<shape->b_count;q++){
                            minx=std::min(minx,shape->big_polygon[q].x*zoom);
                            miny=std::min(miny,shape->big_polygon[q].y*zoom);
                            maxx=std::max(maxx,shape->big_polygon[q].x*zoom);
                            maxy=std::max(maxy,shape->big_polygon[q].y*zoom);
                        }
                        for(int q=0;q<shape->b_count;q++){
                            xp[q]=shape->big_polygon[q].x*zoom-minx;
                            yp[q]=shape->big_polygon[q].y*zoom-miny;
                        }
                        unsigned w=maxx-minx;
                        unsigned h=maxy-miny;
                        int wt,ht;
                        SDL_QueryTexture(find_texture(F_DATA(fix,texture)),0,0,&wt,&ht);
                        wt=wt*zoom/100.0f;
                        ht=ht*zoom/100.0f;

                        SDL_Texture *zoomed=SDL_CreateTexture(ren,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,wt,ht);
                        SDL_SetTextureBlendMode(zoomed,SDL_BLENDMODE_BLEND);
                        SDL_SetRenderTarget(ren,zoomed);
                        SDL_RenderCopy(ren,find_texture(F_DATA(fix,texture)),0,0);
                        SDL_SetRenderTarget(ren,0);

                        prev_tex=shape->cache=SDL_CreateTexture(ren,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w,h);
                        prev_vec=shape->big_polygon;
                        SDL_SetTextureBlendMode(shape->cache,SDL_BLENDMODE_BLEND);
                        SDL_SetRenderTarget(ren,shape->cache);
                        SDL_RenderClear(ren);
                        texturedPolygonTex(ren,xp,yp,shape->b_count,zoomed,0,0);
                        SDL_SetRenderTarget(ren,0);

                        SDL_DestroyTexture(zoomed);
                        delete[]xp;
                        delete[]yp;
                    }
				}
			}
		}
	}
}
