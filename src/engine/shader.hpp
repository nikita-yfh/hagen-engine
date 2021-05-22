#pragma once
#include "sdl.hpp"
#include <string>
#include <vector>
#include <map>
using namespace std;

struct GLSLtype{
	int loc;
	virtual void update(){};
};
struct GLSLfloat : GLSLtype{
	float value;
	void update();
	void set(float v);
};
struct GLSLvec2 : GLSLtype{
	float x;
	float y;
	void update();
	void set(float x,float y);
};
struct GLSLvec3 : GLSLtype{
	float x;
	float y;
	float z;
	void update();
	void set(float x,float y,float z);
};
struct GLSLvec4 : GLSLtype{
	float x;
	float y;
	float z;
	float w;
	void update();
	void set(float x,float y,float z,float w);
};
struct GLSLmat2 : GLSLtype{
	float value[2][2];
	void update();
	void set(float v11,float v12,
			float v21,float v22);
};
struct GLSLmat3 : GLSLtype{
	float value[3][3];
	void update();
	void set(float v11,float v12,float v13,
			float v21,float v22,float v23,
			float v31,float v32,float v33);
};
struct GLSLmat4 : GLSLtype{
	float value[4][4];
	void update();
	void set(float v11,float v12,float v13,float v14,
			float v21,float v22,float v23,float v24,
			float v31,float v32,float v33,float v34,
			float v41,float v42,float v43,float v44);
};
struct GLSLtex : GLSLtype{
	GPU_Image *tex=nullptr;
	void update();
	void set(string name);
};
class Shader {
public:
	// Constructor
	Shader(const string& v_str, const string& f_str);

	Shader();

	void load(const string& v_str, const string& f_str);

	// Destructor
	~Shader();

	// Add an image
	void addImg(string name);

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
	void enable();
	void disable();

	void update();


private:

	// v vertice shader, f fragment shader, l shader program
	uint32_t v, f, l;

	 // Array of pair (id variable, location in the shader)
	map<string,uint32_t> variables;

	// Shader attributes and uniform locations
	GPU_ShaderBlock block;

	// Image for the shader
	GPU_Image *img=nullptr;
};
