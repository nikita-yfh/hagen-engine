#include "text.hpp"
#include "xmlParser.h"
#include "SDL_FontCache.h"
string language="ru";
map <string,string>texts;
string get_text(string id){
	if(texts.empty()){
		XMLNode xml=XMLNode::openFileHelper(("locales/"+language+".xml").c_str(),"text");
		xml=xml.getChildNode(id.c_str());
		return xml.getText();
	}else{
		return texts[id];
	}
}
void preload_locale(){
	XMLNode xml=XMLNode::openFileHelper(("locales/"+language+".xml").c_str(),"text");
	for(int q=0;;q++){
		XMLNode node=xml.getChildNode(q);
		if(node.isEmpty())return;
		texts[node.getName()]=node.getText();
	}
}
void subtitles(string text, float time){

}
