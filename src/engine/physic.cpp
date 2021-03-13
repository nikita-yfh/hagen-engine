#include "physic.hpp"
#include "level.hpp"
#include "utility.hpp"
using namespace std;
b2Body *create_body(string type,string id,float x,float y){
	XMLNode bd=XMLNode::openFileHelper(("templates/"+type+".xml").c_str(),"body");
	b2Body *body=read_body(bd,{x,y});
	if(id=="")id=free_id();
	bodies[id]=body;
	return body;
}
