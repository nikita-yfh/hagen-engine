#include "lua.hpp"
#include "sdl.hpp"
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
		GPU_PolygonFilled(*target,vec.size(),vec.data(),color.color());
	else
		GPU_Polygon(*target,vec.size(),vec.data(),color.color());
	return 0;
}
static int draw_polyline(lua_State *L){
	vector<float>vec=luaval_to_vector_float(L,1);
	Color color=Stack<Color>::get(L,2);
	bool close=false;
	if(lua_gettop(L)>2)
		close=Stack<bool>::get(L,3);
	GPU_Polyline(*target,vec.size(),vec.data(),color.color(),close);
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
	float x=Stack<float>::get(L,1);
	float y=Stack<float>::get(L,2);
	Color color=Stack<Color>::get(L,3);
	GPU_Pixel(*target,x,y,color.color());
	return 0;
}
static GPU_Image* create_texture(int x,int y){
	return GPU_CreateImage(x,y,GPU_FORMAT_RGBA);
}
static int set_target(lua_State *L){
	if(lua_isnil(L,1))
		target=&ren;
	else{
		GPU_Image *image=Stack<GPU_Image*>::get(L,1);
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
static void bind_texture(GPU_Image *tex,string s){
	if(textures[s])
		GPU_FreeImage(textures[s]);
	textures[s]=tex;
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
	GPU_Image *image=Stack<GPU_Image*>::get(L,1);
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
	GPU_Image *image=Stack<GPU_Image*>::get(L,1);
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
	GPU_Image *image=Stack<GPU_Image*>::get(L,1);
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
	Stack::push(x);
	Stack::push(y);
	return 2;
}
void bind_graphics(){
	getGlobalNamespace(L)
	.beginNamespace("graphics")
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
	.addConstant("zero",GPU_FUNC_ZERO)
	.addConstant("one",GPU_FUNC_ONE)
	.addConstant("src_color",GPU_FUNC_SRC_COLOR)
	.addConstant("dst_color",GPU_FUNC_DST_COLOR)
	.addConstant("one_minus_src",GPU_FUNC_ONE_MINUS_SRC)
	.addConstant("one_minus_dst",GPU_FUNC_ONE_MINUS_DST)
	.addConstant("src_alpha",GPU_FUNC_SRC_ALPHA)
	.addConstant("dst_alpha",GPU_FUNC_DST_ALPHA)
	.addConstant("one_minus_src_alpha",GPU_FUNC_ONE_MINUS_SRC_ALPHA)
	.addConstant("one_minus_dst_alpha",GPU_FUNC_ONE_MINUS_DST_ALPHA)
	.endNamespace()
	.beginNamespace("blend_eq")
	.addConstant("add",GPU_EQ_ADD)
	.addConstant("subtract",GPU_EQ_SUBTRACT)
	.addConstant("reverse_subtract",GPU_EQ_REVERSE_SUBTRACT)
    .endNamespace()
    .beginNamespace("blend_preset")
    .addConstant("normal",GPU_BLEND_NORMAL)
    .addConstant("premultiplied_alpha",GPU_BLEND_PREMULTIPLIED_ALPHA)
    .addConstant("multiply",GPU_BLEND_MULTIPLY)
    .addConstant("add",GPU_BLEND_ADD)
    .addConstant("subtract",GPU_BLEND_SUBTRACT)
    .addConstant("mod_alpha",GPU_BLEND_MOD_ALPHA)
    .addConstant("set_alpha",GPU_BLEND_SET_ALPHA)
    .addConstant("set",GPU_BLEND_SET)
    .addConstant("normal_keep_alpha",GPU_BLEND_NORMAL_KEEP_ALPHA)
    .addConstant("normal_add_alpha",GPU_BLEND_NORMAL_ADD_ALPHA)
    .addConstant("normal_factor_alpha",GPU_BLEND_NORMAL_FACTOR_ALPHA)
    .endNamespace()
    .beginNamespace("filter")
    .addConstant("nearest",GPU_FILTER_NEAREST)
    .addConstant("linear",GPU_FILTER_LINEAR)
    .addConstant("linear_mipmap",GPU_FILTER_LINEAR_MIPMAP)
	.endNamespace()
	.beginNamespace("snap")
	.addConstant("none",GPU_SNAP_NONE)
	.addConstant("position",GPU_SNAP_POSITION)
	.addConstant("dimensions",GPU_SNAP_DIMENSIONS)
	.addConstant("position_and_dimensions",GPU_SNAP_POSITION_AND_DIMENSIONS)
	.endNamespace()
	.beginNamespace("wrap")
    .addConstant("none",GPU_WRAP_NONE)
    .addConstant("repeat",GPU_WRAP_REPEAT)
    .addConstant("mirrored",GPU_WRAP_MIRRORED)
	.endNamespace()
	.beginNamespace("format")
	.addConstant("luminance",GPU_FORMAT_LUMINANCE)
	.addConstant("luminance_alpha",GPU_FORMAT_LUMINANCE_ALPHA)
	.addConstant("rgb",GPU_FORMAT_RGB)
	.addConstant("rgba",GPU_FORMAT_RGBA)
	.addConstant("alpha",GPU_FORMAT_ALPHA)
	.addConstant("rg",GPU_FORMAT_RG)
	.addConstant("YCbCr422",GPU_FORMAT_YCbCr422)
	.addConstant("YCbCr420p",GPU_FORMAT_YCbCr420P)
	.addConstant("bgr",GPU_FORMAT_BGR)
	.addConstant("bgra",GPU_FORMAT_BGRA)
	.addConstant("abgr",GPU_FORMAT_ABGR)
	.endNamespace()
	.beginNamespace("file_format")
	.addConstant("auto",GPU_FILE_AUTO)
	.addConstant("png",GPU_FILE_PNG)
	.addConstant("bmp",GPU_FILE_BMP)
	.addConstant("tga",GPU_FILE_TGA)
	.endNamespace()
	.beginNamespace("comparison")
	.addConstant("never",GPU_NEVER)
	.addConstant("less",GPU_LESS)
	.addConstant("greater",GPU_GREATER)
	.addConstant("equal",GPU_EQUAL)
	.addConstant("notequal",GPU_NOTEQUAL)
	.addConstant("lequal",GPU_LEQUAL)
	.addConstant("gequal",GPU_GEQUAL)
	.addConstant("always",GPU_ALWAYS)
	.endNamespace()
	.endNamespace();
}
