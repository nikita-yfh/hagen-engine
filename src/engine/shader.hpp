#pragma once
#include "SDL.h"
#include "SDL_gpu.h"
#include <string>
#include <vector>
#include <map>
using namespace std;
class Shader {
public:
	// Constructor
	Shader(const string& id, const string& v_str, const string& f_str);

	// Destructor
	~Shader();

	// Add an image
	void addImg(string path);

	// Free the image used in the shader
	void freeImg();

	// Add a variable to the vector
	void addVariable(string idV);

	// Get the location of a variable
	uint32_t getVar(string idV);

	// Getter for the id
	string getId();

	// Set the image to the shader
	void setImgShader();

	// Activate the shader
	void activate();


private:
	// Shader Id
	string id;

	// v vertice shader, f fragment shader, p shader program
	uint32_t v, f, l;

	 // Array of pair (id variable, location in the shader)
	map<string,uint32_t> variables;

	// Shader attributes and uniform locations
	GPU_ShaderBlock block;

	// Image for the shader
	GPU_Image *img;
};
