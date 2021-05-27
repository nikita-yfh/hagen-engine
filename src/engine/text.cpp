#include "text.hpp"
#include "main.hpp"
#include "xmlParser.h"
#include "sdl.hpp"
#include "utility.hpp"
#include <iostream>
map <string,string>texts;
string get_text(string id) {
	if(texts.empty() || texts.find(id)==texts.end()) {
		XMLNode xml=XMLNode::openFileHelper((prefix+"locales/"+settings.language+".xml").c_str(),"text");
		string id1=id;
		while(id1.find("/")!=id1.npos) {
			string s(id,0,id1.find("/"));
			id1.erase(id1.begin(),id1.begin()+id1.find("/")+1);
			xml=xml.getChildNode(s.c_str());
		}
		xml=xml.getChildNode(id1.c_str());
		if(!(xml.isEmpty()))
			texts[id]=xml.getAttribute("value");
		else
			warning_log("String "+id+" not found in locale file "+settings.language+".xml");
	}
	return texts[id];
}
const char* get_ctext(string id) {
	get_text(id);
	return texts[id].c_str();
}
void preload_locale() {
	XMLNode xml=XMLNode::openFileHelper((prefix+"locales/"+settings.language+".xml").c_str(),"text");
	for(int q=0;; q++) {
		XMLNode node=xml.getChildNode(q);
		if(node.isEmpty())return;
		texts[node.getName()]=xml.getAttribute("value");
	}
}
void subtitles(string text) {

}
void clear_locale() {
	texts.clear();
}
