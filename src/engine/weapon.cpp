#include "weapon.hpp"
#include "main.hpp"

unordered_map<string,Weapon>weapons;

void load_weapon(){
	XMLNode l=XMLNode::openFileHelper(string(prefix+"weapons.xml").c_str());
	for(int q=0;;q++){
		XMLNode node=l.getChildNode(q);
		if(node.isEmpty())return;
		string id=node.getAttribute("id");
		string type=node.getAttribute("type");
	}
}
