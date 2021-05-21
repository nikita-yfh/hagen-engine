#include "shader.hpp"
#include "utility.hpp"

Shader::Shader(const std::string& id, const std::string& v_str, const std::string& f_str) : id(id), img(NULL) {
	v = GPU_LoadShader(GPU_VERTEX_SHADER, v_str.c_str());

	if (!v)
		error_log((string)"Failed to load vertex shader: "+GPU_GetShaderMessage());

	f = GPU_LoadShader(GPU_FRAGMENT_SHADER, f_str.c_str());

	if (!f)
		error_log((string)"Failed to load fragment shader: "+GPU_GetShaderMessage());

	l = GPU_LinkShaders(v, f);

	if (!l)
		error_log((string)"Failed to link shader program: "+GPU_GetShaderMessage());

	block = GPU_LoadShaderBlock(l, "gpu_Vertex", "gpu_TexCoord", NULL, "gpu_ModelViewProjectionMatrix");
}

Shader::~Shader() {

}

void Shader::freeImg() {
	GPU_FreeImage(img);
}

void Shader::addImg(std::string path) {
	img = GPU_LoadImage(path.c_str());

	GPU_SetSnapMode(img, GPU_SNAP_NONE);
	GPU_SetWrapMode(img, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
}

void Shader::addVariable(std::string idV) {
	uint32_t location = GPU_GetUniformLocation(l, idV.c_str());
	variables[idV]=location;
}

std::string Shader::getId() { return id; }

void Shader::setImgShader() {
	GPU_SetShaderImage(img, getVar("tex1"), 1);
}
void Shader::activate() {
	GPU_ActivateShaderProgram(l, &block);
}

uint32_t Shader::getVar(std::string idV) {
	if(variables.find(idV)!=variables.end())
		return variables[idV];
	return 0;
}
