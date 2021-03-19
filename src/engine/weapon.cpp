#include "weapon.hpp"
#include "main.hpp"

unordered_map<string,Weapon*>weapons;
Weapon::Weapon(string _name){
	name=_name;
}

void load_weapon(){
	XMLNode l=XMLNode::openFileHelper(string(prefix+"weapons.xml").c_str());
	for(int q=0;;q++){
		XMLNode node=l.getChildNode(q);
		if(node.isEmpty())return;
		string id=node.getAttribute("id");
		string type=node.getAttribute("type");
		if(type=="firearms"){

		}
	}
}
