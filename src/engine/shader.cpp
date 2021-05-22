#include "shader.hpp"
#include "utility.hpp"
#include "main.hpp"
#include "sdl.hpp"
#include "camera.hpp"

void GLSLfloat::set(float v){
	value=v;
}
void GLSLfloat::update(){
	GPU_SetUniformf(loc,value);
}
void GLSLvec2::set(float _x,float _y){
	x=_x;
	y=_y;
}
void GLSLvec2::update(){
	float val[2]={x,y};
	GPU_SetUniformfv(loc,2,1,val);
}
void GLSLvec3::set(float _x,float _y,float _z){
	x=_x;
	y=_y;
	z=_z;
}
void GLSLvec3::update(){
	float val[3]={x,y,z};
	GPU_SetUniformfv(loc,3,1,val);
}
void GLSLvec4::set(float _x,float _y,float _z,float _w){
	x=_x;
	y=_y;
	z=_z;
	w=_w;
}
void GLSLvec4::update(){
	float val[4]={x,y,z,w};
	GPU_SetUniformfv(loc,4,1,val);
}
void GLSLmat2::set(float v11,float v12,
					float v21,float v22){
	value[0][0]=v11;value[0][1]=v12;
	value[1][0]=v21;value[1][1]=v22;
}
void GLSLmat2::update(){
	GPU_SetUniformMatrixfv(loc,1,2,2,0,(float*)value);
}
void GLSLmat3::set(float v11,float v12,float v13,
					float v21,float v22,float v23,
					float v31,float v32,float v33){
	value[0][0]=v11;value[0][1]=v12;value[0][2]=v13;
	value[1][0]=v21;value[1][1]=v22;value[1][2]=v23;
	value[2][0]=v31;value[2][1]=v32;value[2][2]=v33;
}
void GLSLmat3::update(){
	GPU_SetUniformMatrixfv(loc,1,3,3,0,(float*)value);
}
void GLSLmat4::set(float v11,float v12,float v13,float v14,
					float v21,float v22,float v23,float v24,
					float v31,float v32,float v33,float v34,
					float v41,float v42,float v43,float v44){
	value[0][0]=v11;value[0][1]=v12;value[0][2]=v13;value[0][3]=v14;
	value[1][0]=v21;value[1][1]=v22;value[1][2]=v23;value[1][3]=v24;
	value[2][0]=v31;value[2][1]=v32;value[2][2]=v33;value[2][3]=v34;
	value[3][0]=v41;value[3][1]=v42;value[3][2]=v43;value[3][3]=v44;
}
void GLSLmat4::update(){
	GPU_SetUniformMatrixfv(loc,1,4,4,0,(float*)value);
}
void GLSLtex::set(string name){
	if(tex)
		GPU_FreeImage(tex);
	tex=GPU_LoadImage((prefix+"textures/"+name).c_str());
}
void GLSLtex::update(){
	GPU_SetShaderImage(tex, loc,1);
}
Shader::Shader(){}
Shader::~Shader(){
	freeImg();
}
void Shader::freeImg(){
	if(img)
		GPU_FreeImage(img);
	img=nullptr;
}
void Shader::load(const std::string& v_str, const std::string& f_str){
	info_log("Vertex shader: "+v_str);
	info_log("Fragment shader: "+f_str);
	v = GPU_LoadShader(GPU_VERTEX_SHADER, v_str.c_str());

	if (!v)
		error_log((string)"Failed to load vertex shader: "+GPU_GetShaderMessage());
	else
		info_log("Loaded vertex shader");

	f = GPU_LoadShader(GPU_FRAGMENT_SHADER, f_str.c_str());

	if (!f)
		error_log((string)"Failed to load fragment shader: "+GPU_GetShaderMessage());
	else
		info_log("Loaded fragment shader");

	l = GPU_LinkShaders(v, f);

	if (!l)
		error_log((string)"Failed to link shader program: "+GPU_GetShaderMessage());
	else
		info_log("Linked shader program");

	block = GPU_LoadShaderBlock(l, "gpu_Vertex", "gpu_TexCoord", "gpu_Color", "gpu_ModelViewProjectionMatrix");
	info_log("Loaded shader block");
}

Shader::Shader(const std::string& v_str, const std::string& f_str) {
	load(v_str,f_str);
}

void Shader::addImg(std::string name) {
	img=GPU_LoadImage((prefix+"textures/"+name).c_str());

	GPU_SetSnapMode(img, GPU_SNAP_NONE);
	GPU_SetWrapMode(img, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
}

void Shader::addVariable(std::string idV) {
	uint32_t location = GPU_GetUniformLocation(l, idV.c_str());
	variables[idV]=location;
}

void Shader::setImgShader() {
	GPU_SetShaderImage(img, getVar("tex1"), 1);
}
void Shader::enable() {
	GPU_ActivateShaderProgram(l, &block);
	update();
}
void Shader::disable() {
	GPU_DeactivateShaderProgram();
}

uint32_t Shader::getVar(std::string idV) {
	if(variables.find(idV)!=variables.end())
		return variables[idV];
	addVariable(idV);
	return getVar(idV);
}

void Shader::update(){
	GPU_SetUniformf(getVar("time"), lua::get_time());
	GPU_SetUniformf(getVar("zoom"), zoom);
	setImgShader();
	float resolution[2]={SW,SH};
	GPU_SetUniformfv(getVar("resolution"), 2, 1, resolution);
}
