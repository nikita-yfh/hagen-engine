#include "SDL.h"
#include <string>
#include <vector>
extern SDL_Event e;
extern SDL_Renderer *ren;
extern SDL_Window *window;
extern const uint8_t* key;
extern int SW;
extern int SH;
void init(const char* title,int w,int h);
void quit();
void panic(std::string name,std::string message);
SDL_Texture *find_texture(std::string id);
struct Texture{
    SDL_Texture *texture;
    std::string id;
};
extern std::vector<Texture>textures;
extern SDL_Texture *background;
void load_cursor();
void load_textures();
void configure_textures();
void destroy_textures();
void load_background(std::string path);
void create_cache();
