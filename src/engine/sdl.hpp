#include "SDL.h"
#include <string>
#include <vector>
#include "SDL_gpu.h"
extern SDL_Event e;
extern GPU_Target *ren;
extern const uint8_t* key;
extern int SW;
extern int SH;
void init(const char* title,int w,int h);
void quit();
void panic(std::string name,std::string message);
GPU_Image *find_texture(std::string id);
struct Texture {
	GPU_Image *texture;
	std::string id;
};
extern std::vector<Texture>textures;
extern GPU_Image *background;
void load_cursor();
void load_textures();
void configure_textures();
void destroy_textures();
void load_background(std::string path);
void create_cache();
