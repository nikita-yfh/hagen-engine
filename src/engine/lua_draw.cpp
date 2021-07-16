#include "lua_draw.hpp"
#include "camera.hpp"
#include "effect.hpp"
#include "shader.hpp"
#include "main.hpp"
#include "render.hpp"
using namespace lua;
using namespace luabridge;
using namespace std;
extern GPU_Target *ren1;
static GPU_Target **target=&ren1;

static int draw_rect(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	float x1=Stack<float>::get(L,2);
	float y1=Stack<float>::get(L,3);
	float x2=Stack<float>::get(L,4);
	float y2=Stack<float>::get(L,5);
	bool fill=false;
	float round=0.0f;
	if(lua_gettop(L)>5)
		fill=Stack<bool>::get(L,6);
	if(lua_gettop(L)>6)
		round=Stack<float>::get(L,7);
	if(fill)
		GPU_RectangleRoundFilled(*target,x1,y1,x2,y2,round,color.color());
	else
		GPU_RectangleRound(*target,x1,y1,x2,y2,round,color.color());
	return 0;
}
static int draw_polygon(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	vector<float>vec=luaval_to_vector_float(L,2);
	bool fill=false;
	if(lua_gettop(L)>2)
		fill=Stack<bool>::get(L,3);
	if(fill)
		GPU_PolygonFilled(*target,vec.size()/2,vec.data(),color.color());
	else
		GPU_Polygon(*target,vec.size()/2,vec.data(),color.color());
	return 0;
}
static int draw_polyline(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	vector<float>vec=luaval_to_vector_float(L,2);
	bool close=false;
	if(lua_gettop(L)>2)
		close=Stack<bool>::get(L,3);
	GPU_Polyline(*target,vec.size()/2,vec.data(),color.color(),close);
	return 0;
}
static int draw_tri(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	float x1=Stack<float>::get(L,2);
	float y1=Stack<float>::get(L,3);
	float x2=Stack<float>::get(L,4);
	float y2=Stack<float>::get(L,5);
	float x3=Stack<float>::get(L,6);
	float y3=Stack<float>::get(L,7);
	bool fill=false;
	if(lua_gettop(L)>7)
		fill=Stack<bool>::get(L,8);
	if(fill)
		GPU_TriFilled(*target,x1,y1,x2,y2,x3,y3,color.color());
	else
		GPU_Tri(*target,x1,y1,x2,y2,x3,y3,color.color());
	return 0;
}
static int draw_sector(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	float x=Stack<float>::get(L,2);
	float y=Stack<float>::get(L,3);
	float r1=Stack<float>::get(L,4);
	float r2=Stack<float>::get(L,5);
	float a1=Stack<float>::get(L,6);
	float a2=Stack<float>::get(L,7);
	bool fill=false;
	if(lua_gettop(L)>7)
		fill=Stack<bool>::get(L,8);
	if(fill)
		GPU_SectorFilled(*target,x,y,r1,r2,a1,a2,color.color());
	else
		GPU_Sector(*target,x,y,r1,r2,a1,a2,color.color());
	return 0;
}
static int draw_ellipse(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	float x=Stack<float>::get(L,2);
	float y=Stack<float>::get(L,3);
	float r1=Stack<float>::get(L,4);
	float r2=Stack<float>::get(L,5);
	float a=Stack<float>::get(L,6);
	bool fill=false;
	if(lua_gettop(L)>6)
		fill=Stack<bool>::get(L,7);
	if(fill)
		GPU_EllipseFilled(*target,x,y,r1,r2,a,color.color());
	else
		GPU_Ellipse(*target,x,y,r1,r2,a,color.color());
	return 0;
}
static int draw_circle(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	float x=Stack<float>::get(L,2);
	float y=Stack<float>::get(L,3);
	float r=Stack<float>::get(L,4);
	bool fill=false;
	if(lua_gettop(L)>4)
		fill=Stack<bool>::get(L,5);
	if(fill)
		GPU_CircleFilled(*target,x,y,r,color.color());
	else
		GPU_Circle(*target,x,y,r,color.color());
	return 0;
}

static int draw_arc(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	float x=Stack<float>::get(L,2);
	float y=Stack<float>::get(L,3);
	float r=Stack<float>::get(L,4);
	float a1=Stack<float>::get(L,5);
	float a2=Stack<float>::get(L,6);
	bool fill=false;
	if(lua_gettop(L)>6)
		fill=Stack<bool>::get(L,7);
	if(fill)
		GPU_ArcFilled(*target,x,y,r,a1,a2,color.color());
	else
		GPU_Arc(*target,x,y,r,a1,a2,color.color());
	return 0;
}

static int draw_line(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	float x1=Stack<float>::get(L,2);
	float y1=Stack<float>::get(L,3);
	float x2=Stack<float>::get(L,4);
	float y2=Stack<float>::get(L,5);
	GPU_Line(*target,x1,y1,x2,y2,color.color());
	return 0;
}
static int draw_pixel(lua_State *L){
	Color color=Stack<Color>::get(L,1);
	float x=Stack<float>::get(L,2);
	float y=Stack<float>::get(L,3);
	GPU_Pixel(*target,x,y,color.color());
	return 0;
}
static Texture* create_texture(int x,int y){
	return (Texture*)GPU_CreateImage(x,y,GPU_FORMAT_RGBA);
}
static int set_target(lua_State *L){
	if(lua_isnil(L,1))
		target=&ren;
	else{
		GPU_Image *image=(Texture*)(GPU_Image*)Stack<Texture*>::get(L,1);
		if(image->target==0)
			GPU_LoadTarget(image);
		target=&image->target;
	}
	return 0;
}
static int unset_target(lua_State *L){
	target=&ren;
	return 0;
}
static void bind_texture(Texture *tex,string s){
	if(textures[s])
		GPU_FreeImage(textures[s]);
	textures[s]=(GPU_Image*)tex;
}
static void set_line_thickness(float thickness){
	GPU_SetLineThickness(thickness);
}
static void set_viewport(float x,float y,float w,float h){
	GPU_SetViewport(*target,GPU_MakeRect(x,y,w,h));
}
static void unset_viewport(){
	GPU_UnsetViewport(*target);
}
static Color get_pixel(int x,int y){
	return GPU_GetPixel(*target,x,SH-y);
}
static void set_clip(int x,int y,int w,int h){
	GPU_SetClip(*target,x,y,w,h);
}
static void unset_clip(){
	GPU_UnsetClip(*target);
}
static int set_color(lua_State *L){
	GPU_Image *image=(GPU_Image*)Stack<Texture*>::get(L,1);
	if(lua_isnil(L,2))
		GPU_UnsetColor(image);
	else{
		Color color=Stack<Color>::get(L,2);
		GPU_SetColor(image,color.color());
	}
	return 0;
}
static int unset_color(lua_State *L){
	GPU_Image *image=Stack<GPU_Image*>::get(L,1);
	GPU_UnsetColor(image);
	return 0;
}
static int blit(lua_State *L){
	GPU_Image *image=Stack<Texture*>::get(L,1);
	float x=Stack<float>::get(L,2);
	float y=Stack<float>::get(L,3);
	float angle=0.0f;
	float scalex=1.0f,scaley=1.0f;
	float pivotx=image->w*image->anchor_x,pivoty=image->h*image->anchor_y;
	if(lua_gettop(L)>3)
		angle=Stack<float>::get(L,4);
	if(lua_gettop(L)>4){
		scalex=Stack<float>::get(L,5);
		scaley=Stack<float>::get(L,6);
	}
	if(lua_gettop(L)>6){
		pivotx=Stack<float>::get(L,7);
		pivoty=Stack<float>::get(L,8);
	}
	GPU_BlitTransformX(image,0,*target,x,y,pivotx,pivoty,angle,scalex,scaley);
	return 0;
}
static int blit_rect(lua_State *L){
	GPU_Image *image=(GPU_Image*)Stack<Texture*>::get(L,1);
	float x=Stack<float>::get(L,2);
	float y=Stack<float>::get(L,3);
	GPU_Rect rect=GPU_MakeRect(
		Stack<float>::get(L,4),
		Stack<float>::get(L,5),
		Stack<float>::get(L,6),
		Stack<float>::get(L,7)
	);
	float angle=0.0f;
	float scalex=1.0f,scaley=1.0f;
	float pivotx=rect.w*image->anchor_x,pivoty=rect.h*image->anchor_y;
	if(lua_gettop(L)>7)
		angle=Stack<float>::get(L,8);
	if(lua_gettop(L)>8){
		scalex=Stack<float>::get(L,9);
		scaley=Stack<float>::get(L,10);
	}
	if(lua_gettop(L)>10){
		pivotx=Stack<float>::get(L,11);
		pivoty=Stack<float>::get(L,12);
	}
	GPU_BlitTransformX(image,&rect,*target,x,y,pivotx,pivoty,angle,scalex,scaley);
	return 0;
}
static void clear(){
	GPU_Clear(*target);
}
static int get_default_anchor(lua_State *L){
	float x,y;
	GPU_GetDefaultAnchor(&x,&y);
	push(L,x);
	push(L,y);
	return 2;
}

uint16_t Texture::GetW()const{return w;}
uint16_t Texture::GetH()const{return h;}
uint16_t Texture::GetTexW()const{return texture_w;}
uint16_t Texture::GetTexH()const{return texture_h;}
int Texture::GetSnapMode()const{return snap_mode;}
void Texture::SetSnapMode(int v){GPU_SetSnapMode(this,(GPU_SnapEnum)v);}
BlendModeWrapper Texture::GetBlendMode()const{return *((BlendModeWrapper*)(&blend_mode));}
void Texture::SetBlendMode(BlendModeWrapper v){
	GPU_BlendMode b=*((GPU_BlendMode*)(&v));
    GPU_SetBlendFunction(this, b.source_color, b.dest_color, b.source_alpha, b.dest_alpha);
    GPU_SetBlendEquation(this, b.color_equation, b.alpha_equation);
}
int Texture::GetBytesPerPixel()const{return bytes_per_pixel;}
int Texture::GetRefCount()const{return refcount;}
int Texture::GetNumLayers()const{return num_layers;}
bool Texture::UseBlending()const{return use_blending;}
void Texture::SetBlending(bool v){GPU_SetBlending(this,v);}
bool Texture::IsAlias()const{return is_alias;}
bool Texture::HasMipmaps()const{return has_mipmaps;}
bool Texture::UsingVirtualResolution()const{return using_virtual_resolution;}
int Texture::GetFilterMode()const{return filter_mode;}
void Texture::SetFilterMode(int v){GPU_SetImageFilter(this,(GPU_FilterEnum)v);}
int Texture::GetWrapModeX()const{return wrap_mode_x;}
int Texture::GetWrapModeY()const{return wrap_mode_y;}
void Texture::SetWrapModeX(int v){GPU_SetWrapMode(this,(GPU_WrapEnum)v,wrap_mode_y);}
void Texture::SetWrapModeY(int v){GPU_SetWrapMode(this,wrap_mode_x,(GPU_WrapEnum)v);}
float Texture::GetAnchorX()const{return anchor_x;}
float Texture::GetAnchorY()const{return anchor_y;}
void Texture::SetAnchorX(float v){GPU_SetAnchor(this,v,anchor_y);}
void Texture::SetAnchorY(float v){GPU_SetAnchor(this,anchor_x,v);}

int BlendModeWrapper::GetSC()const{return source_color;}
int BlendModeWrapper::GetSA()const{return source_alpha;}
int BlendModeWrapper::GetDC()const{return dest_color;}
int BlendModeWrapper::GetDA()const{return dest_alpha;}
int BlendModeWrapper::GetCE()const{return color_equation;}
int BlendModeWrapper::GetAE()const{return alpha_equation;}
void BlendModeWrapper::SetSC(int v){source_color=(GPU_BlendFuncEnum)v;}
void BlendModeWrapper::SetSA(int v){source_alpha=(GPU_BlendFuncEnum)v;}
void BlendModeWrapper::SetDC(int v){dest_color=(GPU_BlendFuncEnum)v;}
void BlendModeWrapper::SetDA(int v){dest_alpha=(GPU_BlendFuncEnum)v;}
void BlendModeWrapper::SetCE(int v){color_equation=(GPU_BlendEqEnum)v;}
void BlendModeWrapper::SetAE(int v){alpha_equation=(GPU_BlendEqEnum)v;}

static Texture *find_tex(string str){
	if(textures.find(str)==textures.end())
		return 0;
	return (Texture*)textures[str];
}
static Texture *load_tex(string str){
	return (Texture*)load_texture(str);
}
static float get_ddpi(){
	float f;
	if(SDL_GetDisplayDPI(0,&f,0,0))
		panic(SDL_GetError());
	return f;
}
static float get_vdpi(){
	float f;
	if(SDL_GetDisplayDPI(0,0,0,&f))
		panic(SDL_GetError());
	return f;
}
static float get_hdpi(){
	float f;
	if(SDL_GetDisplayDPI(0,0,&f,0))
		panic(SDL_GetError());
	return f;
}
static float get_dpi(){
	float f1,f2;
	if(SDL_GetDisplayDPI(0,0,&f1,&f2))
		panic(SDL_GetError());
	return (f1+f2)/2.0f;
}
static float get_refresh_rate(){
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0,&mode);
	return mode.refresh_rate;
}
void bind_graphics(){
	getGlobalNamespace(L)
	.beginNamespace("graphics")
	.beginClass<GLSLtype>("GLSLtype")	//базовый тип GLSL
	.addFunction("update",&GLSLtype::update)
	.addProperty("location",&GLSLtype::loc,0)
	.endClass()
	.deriveClass<GLSLfloat,GLSLtype>("GLSLfloat")
	.addFunction("set",&GLSLfloat::set)
	.addProperty("value",&GLSLfloat::value)
	.endClass()
	.deriveClass<GLSLint,GLSLtype>("GLSLint")
	.addFunction("set",&GLSLint::set)
	.addProperty("value",&GLSLint::value)
	.endClass()
	.deriveClass<GLSLuint,GLSLtype>("GLSLuint")
	.addFunction("set",&GLSLuint::set)
	.addProperty("value",&GLSLuint::value)
	.endClass()
	.deriveClass<GLSLtex,GLSLtype>("GLSLtex")	//текстура
	.addFunction("set",&GLSLtex::set)
	.addFunction("set_tex",&GLSLtex::set_tex)
	.addProperty("value",&GLSLtex::tex)
	.endClass()
	.deriveClass<GLSLvec2,GLSLtype>("GLSLvec2")	//двумерный вектор
	.addFunction("set",&GLSLvec2::set)
	.addProperty("x",&GLSLvec2::x)
	.addProperty("y",&GLSLvec2::y)
	.addProperty("r",&GLSLvec2::x)
	.addProperty("g",&GLSLvec2::y)
	.endClass()
	.deriveClass<GLSLvec3,GLSLtype>("GLSLvec3")	//трехмерный вектор
	.addFunction("set",&GLSLvec3::set)
	.addProperty("x",&GLSLvec3::x)
	.addProperty("y",&GLSLvec3::y)
	.addProperty("z",&GLSLvec3::z)
	.addProperty("r",&GLSLvec3::x)
	.addProperty("g",&GLSLvec3::y)
	.addProperty("b",&GLSLvec3::z)
	.endClass()
	.deriveClass<GLSLvec4,GLSLtype>("GLSLvec4")	//четырехмерный вектор
	.addFunction("set",&GLSLvec4::set)
	.addProperty("x",&GLSLvec4::x)
	.addProperty("y",&GLSLvec4::y)
	.addProperty("z",&GLSLvec4::z)
	.addProperty("w",&GLSLvec4::w)
	.addProperty("r",&GLSLvec4::x)
	.addProperty("g",&GLSLvec4::y)
	.addProperty("b",&GLSLvec4::z)
	.addProperty("a",&GLSLvec4::w)
	.endClass()
	.beginClass<Shader>("Shader")
	.addConstructor <void (*) (string,string)> ()
	.addFunction("add_tex",&Shader::add_tex)
	.addFunction("add_float",&Shader::add_float)
	.addFunction("add_int",&Shader::add_int)
	.addFunction("add_uint",&Shader::add_uint)
	.addFunction("add_vec2",&Shader::add_vec2)
	.addFunction("add_vec3",&Shader::add_vec3)
	.addFunction("add_vec4",&Shader::add_vec4)
	.addFunction("add_mat2",&Shader::add_mat2)
	.addFunction("add_mat3",&Shader::add_mat3)
	.addFunction("add_mat4",&Shader::add_mat4)
	.endClass()
	.beginNamespace("display")
	.addProperty("w",&SW,0)
	.addProperty("h",&SH,0)
	.addProperty("dpi",&get_dpi)
	.addProperty("ddpi",&get_ddpi)
	.addProperty("vdpi",&get_vdpi)
	.addProperty("hdpi",&get_hdpi)
	.addProperty("refresh_rate",&get_refresh_rate)
	.endNamespace()
	.addFunction("drawx",&drawx)
	.addFunction("drawy",&drawy)
	.addFunction("preload",&load_tex)	//загрузка текстуры. Позволяет избежать фризов в игре, если все загрузить сразу
	.addFunction("texture",&find_tex)	//текстура по ID
	.addFunction("effect",&effect::create)	//создание граффического эффекта в заданных координатах.
	.addFunction("set_texture_shader",&set_texture_shader)
	.addFunction("unset_texture_shader",&unset_texture_shader)
	.addFunction("get_shader",&get_shader)
	.addProperty("texture_scale",&tex_scale)
	.addProperty("weapon_scale",&weapon_scale)
	.addProperty("effect_scale",&effect_scale)
	.beginClass<Texture>("Texture")
	.addProperty("w",&Texture::GetW)
	.addProperty("h",&Texture::GetH)
	.addProperty("tex_w",&Texture::GetTexW)
	.addProperty("tex_h",&Texture::GetTexH)
	.addProperty("wrap_x",&Texture::GetWrapModeX,&Texture::SetWrapModeX)
	.addProperty("wrap_y",&Texture::GetWrapModeY,&Texture::SetWrapModeY)
	.addProperty("anchor_x",&Texture::GetAnchorX,&Texture::SetAnchorX)
	.addProperty("anchor_y",&Texture::GetAnchorY,&Texture::SetAnchorY)
	.addProperty("blend_mode",&Texture::GetBlendMode,&Texture::SetBlendMode)
	.addProperty("filter",&Texture::GetFilterMode,&Texture::SetFilterMode)
	.addProperty("blending",&Texture::UseBlending,&Texture::SetBlending)
	.addProperty("bytes_per_pixel",&Texture::GetBytesPerPixel)
	.addProperty("has_mipmap",&Texture::HasMipmaps)
	.addProperty("is_alias",&Texture::IsAlias)
	.addProperty("num_layers",&Texture::GetNumLayers)
	.addProperty("refcount",&Texture::GetRefCount)
	.addProperty("use_virtual_resolution",&Texture::UsingVirtualResolution)
	.endClass()
	.beginClass<BlendModeWrapper>("BlendMode")
	.addProperty("source_color",&BlendModeWrapper::GetSC,&BlendModeWrapper::SetSC)
	.addProperty("source_alpha",&BlendModeWrapper::GetSA,&BlendModeWrapper::SetSA)
	.addProperty("dest_color",&BlendModeWrapper::GetDC,&BlendModeWrapper::SetDC)
	.addProperty("dest_alpha",&BlendModeWrapper::GetDA,&BlendModeWrapper::SetDA)
	.addProperty("color_eq",&BlendModeWrapper::GetCE,&BlendModeWrapper::SetCE)
	.addProperty("alpha_eq",&BlendModeWrapper::GetAE,&BlendModeWrapper::SetAE)
	.endClass()
	.addFunction("clear",&clear)
	.addFunction("rect",&draw_rect)
	.addFunction("polygon",&draw_polygon)
	.addFunction("polyline",&draw_polyline)
	.addFunction("circle",&draw_circle)
	.addFunction("pixel",&draw_pixel)
	.addFunction("line",&draw_line)
	.addFunction("triangle",&draw_tri)
	.addFunction("arc",&draw_arc)
	.addFunction("sector",&draw_sector)
	.addFunction("set_target",&set_target)
	.addFunction("unset_target",&unset_target)
	.addFunction("create_texture",&create_texture)
	.addFunction("destroy_texture",&GPU_FreeImage)
	.addFunction("bind_texture",&bind_texture)
	.addFunction("set_viewport",set_viewport)
	.addFunction("unset_viewport",unset_viewport)
	.addFunction("set_clip",set_clip)
	.addFunction("unset_clip",unset_clip)
	.addFunction("get_pixel",get_pixel)
	.addFunction("set_texture_color",set_color)
	.addFunction("unset_texture_color",unset_color)
	.addFunction("copy_texture",GPU_CopyImage)
	.addFunction("set_blending",GPU_SetBlending)
	.addFunction("get_blending",GPU_GetBlending)
	.addFunction("blit",blit)
	.addFunction("blit_rect",blit_rect)
	.addFunction("set_default_anchor",GPU_SetDefaultAnchor)
	.addFunction("get_default_anchor",get_default_anchor)
	.addProperty("line_thickness",GPU_GetLineThickness,set_line_thickness)
	.beginNamespace("blend_func")
	.addConstant("zero",(int)GPU_FUNC_ZERO)
	.addConstant("one",(int)GPU_FUNC_ONE)
	.addConstant("src_color",(int)GPU_FUNC_SRC_COLOR)
	.addConstant("dst_color",(int)GPU_FUNC_DST_COLOR)
	.addConstant("one_minus_src",(int)GPU_FUNC_ONE_MINUS_SRC)
	.addConstant("one_minus_dst",(int)GPU_FUNC_ONE_MINUS_DST)
	.addConstant("src_alpha",(int)GPU_FUNC_SRC_ALPHA)
	.addConstant("dst_alpha",(int)GPU_FUNC_DST_ALPHA)
	.addConstant("one_minus_src_alpha",(int)GPU_FUNC_ONE_MINUS_SRC_ALPHA)
	.addConstant("one_minus_dst_alpha",(int)GPU_FUNC_ONE_MINUS_DST_ALPHA)
	.endNamespace()
	.beginNamespace("blend_eq")
	.addConstant("add",(int)GPU_EQ_ADD)
	.addConstant("subtract",(int)GPU_EQ_SUBTRACT)
	.addConstant("reverse_subtract",(int)GPU_EQ_REVERSE_SUBTRACT)
    .endNamespace()
    .beginNamespace("blend_preset")
    .addConstant("normal",(int)GPU_BLEND_NORMAL)
    .addConstant("premultiplied_alpha",(int)GPU_BLEND_PREMULTIPLIED_ALPHA)
    .addConstant("multiply",(int)GPU_BLEND_MULTIPLY)
    .addConstant("add",(int)GPU_BLEND_ADD)
    .addConstant("subtract",(int)GPU_BLEND_SUBTRACT)
    .addConstant("mod_alpha",(int)GPU_BLEND_MOD_ALPHA)
    .addConstant("set_alpha",(int)GPU_BLEND_SET_ALPHA)
    .addConstant("set",(int)GPU_BLEND_SET)
    .addConstant("normal_keep_alpha",(int)GPU_BLEND_NORMAL_KEEP_ALPHA)
    .addConstant("normal_add_alpha",(int)GPU_BLEND_NORMAL_ADD_ALPHA)
    .addConstant("normal_factor_alpha",(int)GPU_BLEND_NORMAL_FACTOR_ALPHA)
    .endNamespace()
    .beginNamespace("filter")
    .addConstant("nearest",(int)GPU_FILTER_NEAREST)
    .addConstant("linear",(int)GPU_FILTER_LINEAR)
    .addConstant("linear_mipmap",(int)GPU_FILTER_LINEAR_MIPMAP)
	.endNamespace()
	.beginNamespace("snap")
	.addConstant("none",(int)GPU_SNAP_NONE)
	.addConstant("position",(int)GPU_SNAP_POSITION)
	.addConstant("dimensions",(int)GPU_SNAP_DIMENSIONS)
	.addConstant("position_and_dimensions",(int)GPU_SNAP_POSITION_AND_DIMENSIONS)
	.endNamespace()
	.beginNamespace("wrap")
    .addConstant("none",(int)GPU_WRAP_NONE)
    .addConstant("repeat",(int)GPU_WRAP_REPEAT)
    .addConstant("mirrored",(int)GPU_WRAP_MIRRORED)
	.endNamespace()
	.beginNamespace("format")
	.addConstant("luminance",(int)GPU_FORMAT_LUMINANCE)
	.addConstant("luminance_alpha",(int)GPU_FORMAT_LUMINANCE_ALPHA)
	.addConstant("rgb",(int)GPU_FORMAT_RGB)
	.addConstant("rgba",(int)GPU_FORMAT_RGBA)
	.addConstant("alpha",(int)GPU_FORMAT_ALPHA)
	.addConstant("rg",(int)GPU_FORMAT_RG)
	.addConstant("YCbCr422",(int)GPU_FORMAT_YCbCr422)
	.addConstant("YCbCr420p",(int)GPU_FORMAT_YCbCr420P)
	.endNamespace()
	.beginNamespace("file_format")
	.addConstant("auto",(int)GPU_FILE_AUTO)
	.addConstant("png",(int)GPU_FILE_PNG)
	.addConstant("bmp",(int)GPU_FILE_BMP)
	.addConstant("tga",(int)GPU_FILE_TGA)
	.endNamespace()
	.endNamespace();
}
