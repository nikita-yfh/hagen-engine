#include "text.hpp"
#include "main.hpp"
#include "xmlParser.h"
#include "SDL_FontCache.h"
string language="ru";
map <string,string>texts;
string get_text(string id) {
	if(texts.empty() || texts.find(id)!=texts.end()) {
		XMLNode xml=XMLNode::openFileHelper((prefix+"locales/"+language+".xml").c_str(),"text");
		string id1=id;
		while(id1.find("/")!=id1.npos) {
			string s(id,0,id1.find("/"));
			id1.erase(id1.begin(),id1.begin()+id1.find("/")+1);
			xml=xml.getChildNode(s.c_str());
		}
		xml=xml.getChildNode(id1.c_str());
		if(!(xml.isEmpty()))
			texts[id]=xml.getText();
	}
	return texts[id];
}
void preload_locale() {
	XMLNode xml=XMLNode::openFileHelper((prefix+"locales/"+language+".xml").c_str(),"text");
	for(int q=0;; q++) {
		XMLNode node=xml.getChildNode(q);
		if(node.isEmpty())return;
		texts[node.getName()]=node.getText();
	}
}
void subtitles(string text) {

}
