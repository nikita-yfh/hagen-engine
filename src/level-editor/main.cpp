#include <gtk/gtk.h>
#include <cmath>
#include <algorithm>
#include "events.hpp"
#include "editor.hpp"
#include "level.hpp"
int main(int argc,char **args) {
	gtk_init(&argc,&args);
	level.entities.push_back(new Entity);
	level.entities[0]->x=5;
	level.entities[0]->y=5;
	level.entities[0]->id="entity";
	init();
	gtk_main();
	return 0;
}
