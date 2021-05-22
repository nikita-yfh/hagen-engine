#include "shader.hpp"
#include "utility.hpp"
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
