#include "weapon.hpp"
#include "main.hpp"
#include "sdl.hpp"
#include <fstream>
unordered_map<string,Weapon>weapons;
void Weapon::set_texture(string tex){
	if(textures.find(tex)==textures.end())
		load_texture(tex);
	texture=tex;
	printf("texture: %s\n",texture.c_str());
}
string Weapon::get_texture() const{
	return texture;
}
