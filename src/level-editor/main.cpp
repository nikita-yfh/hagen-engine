#include <gtk/gtk.h>
#include <cmath>
#include <algorithm>
#include "events.hpp"
#include "editor.hpp"
#include "level.hpp"
int main(int argc,char **args) {
	gtk_init(&argc,&args);
	init();
	gtk_main();
	return 0;
}
