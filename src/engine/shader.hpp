#pragma once
#include "sdl.hpp"
#include "lua_draw.hpp"
#include <string>
#include <vector>
#include <map>
using namespace std;
struct GLSLtype {
	uint loc;
	virtual void update() {};
};
struct GLSLfloat : GLSLtype {
	float value;
	void update();
	void set(float v);
};
struct GLSLint : GLSLtype {
	int value;
	void update();
	void set(int v);
};
struct GLSLuint : GLSLtype {
	uint value;
	void update();
	void set(uint v);
};
struct GLSLvec2 : GLSLtype {
	float x;
	float y;
	void update();
	void set(float x,float y);
};
struct GLSLvec3 : GLSLtype {
	float x;
	float y;
	float z;
	void update();
	void set(float x,float y,float z);
};
struct GLSLvec4 : GLSLtype {
	float x;
	float y;
	float z;
	float w;
	void update();
	void set(float x,float y,float z,float w);
};
struct GLSLmat2 : GLSLtype {
	float value[2][2];
	void update();
	void set(float v11,float v12,
			 float v21,float v22);
};
struct GLSLmat3 : GLSLtype {
	float value[3][3];
	void update();
	void set(float v11,float v12,float v13,
			 float v21,float v22,float v23,
			 float v31,float v32,float v33);
};
struct GLSLmat4 : GLSLtype {
	float value[4][4];
	void update();
	void set(float v11,float v12,float v13,float v14,
			 float v21,float v22,float v23,float v24,
			 float v31,float v32,float v33,float v34,
			 float v41,float v42,float v43,float v44);
};
struct GLSLtex : GLSLtype {
	GPU_Image *tex=nullptr;
	void update();
	void set(string name);
	void set_tex(Texture *tex);
	~GLSLtex();
};
class Shader {
public:
	Shader();
	~Shader();
	Shader(string v_str, string f_str);
	void load(string v_str, string f_str);

	GLSLtype *getuniform(string idV);

	GLSLfloat *add_float(string id);
	GLSLint *add_int(string id);
	GLSLuint *add_uint(string id);
	GLSLvec2 *add_vec2(string id);
	GLSLvec3 *add_vec3(string id);
	GLSLvec4 *add_vec4(string id);
	GLSLmat2 *add_mat2(string id);
	GLSLmat3 *add_mat3(string id);
	GLSLmat4 *add_mat4(string id);
	GLSLtex *add_tex(string id);

	void enable();
	void disable();

	void update();


private:
	uint32_t v, f, l;
	map<string,GLSLtype*> variables;
	GPU_ShaderBlock block;
	GPU_Image *img=nullptr;
};
Shader *get_shader(string id);
void set_texture_shader(string id,Shader shader);
void unset_texture_shader(string id);
void enable_shader(string id);
void disable_shaders();
