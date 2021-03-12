#include "SDL.h"
#include <string>
#include <map>
#include "SDL_gpu.h"
#define GAME_KEY_COUNT 21
extern SDL_Event e;
extern GPU_Target *ren;
extern const uint8_t *key;
extern int SW;
extern int SH;
void init(const char* title,int w,int h);
void quit();
void panic(std::string name,std::string message);
GPU_Image *find_texture(std::string id);
extern std::map<std::string,GPU_Image*>textures;
extern GPU_Image *background;
void load_cursor();
void load_textures();
void configure_textures();
void destroy_textures();
void load_background(std::string path);
