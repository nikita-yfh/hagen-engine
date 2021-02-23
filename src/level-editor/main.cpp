#include <gtk/gtk.h>
#include <cmath>
#include <algorithm>
#include "events.h"
#include "editor.h"
#include "level.h"
int main(int argc,char **args) {
	gtk_init(&argc,&args);
	init();
	gtk_main();
	return 0;
}
