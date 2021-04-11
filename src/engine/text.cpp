#include "text.hpp"
#include "main.hpp"
#include "xmlParser.h"
#include "SDL_FontCache.h"
string language="ru";
unordered_map <string,string>texts;
string get_text(string id){
	if(texts.empty() || texts.find(id)!=texts.end()){
		XMLNode xml=XMLNode::openFileHelper((prefix+"locales/"+language+".xml").c_str(),"text");
		xml=xml.getChildNode(id.c_str());
		if(!(xml.isEmpty()))
			texts[id]=xml.getText();
	}
	return texts[id];
}
void preload_locale(){
	XMLNode xml=XMLNode::openFileHelper((prefix+"locales/"+language+".xml").c_str(),"text");
	for(int q=0;;q++){
		XMLNode node=xml.getChildNode(q);
		if(node.isEmpty())return;
		texts[node.getName()]=node.getText();
	}
}
void subtitles(string text, float time){

}
