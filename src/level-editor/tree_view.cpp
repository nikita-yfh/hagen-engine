#include <gtk/gtk.h>
#include "utils.hpp"
#include "editor.hpp"
#include "events.hpp"
#include "level.hpp"
#include "body.hpp"
#include "tree_view.hpp"
#include "object.hpp"
#include <vector>
using namespace std;
void fill_shapes() {
	gtk_tree_store_clear(store);
	GtkTreeIter p;
	/*gtk_tree_store_append(store, &p, 0);
	gtk_tree_store_set(store, &p, 0, level.start.id.c_str(), 1, level.start.name().c_str(), -1);
	gtk_tree_store_append(store, &p, 0);
	gtk_tree_store_set(store, &p, 0, level.end.id.c_str(), 1, level.end.name().c_str(), -1);*/
	for(int q=0; q<level.bodies.size(); q++) {
		gtk_tree_store_append(store, &p, 0);
		gtk_tree_store_set(store, &p, 0, level.bodies[q]->id.c_str(), 1, level.bodies[q]->name().c_str(), -1);
		for(int e=0; e<level.bodies[q]->shapes.size(); e++) {
			GtkTreeIter p1;
			gtk_tree_store_append(store, &p1, &p);
			gtk_tree_store_set(store, &p1, 0, level.bodies[q]->shapes[e]->id.c_str(), 1, level.bodies[q]->shapes[e]->name().c_str(), -1);
		}
	}
	for(int q=0; q<level.joints.size(); q++) {
		gtk_tree_store_append(store, &p, 0);
		gtk_tree_store_set(store, &p, 0, level.joints[q]->id.c_str(), 1, level.joints[q]->name().c_str(), -1);
	}
	for(int q=0; q<level.entities.size(); q++) {
		gtk_tree_store_append(store, &p, 0);
		gtk_tree_store_set(store, &p, 0, level.entities[q]->id.c_str(), 1, level.entities[q]->name().c_str(), -1);
	}
}
void select_object() {
	if(block)return;
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(shape_view));
	GtkTreeSelection *selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(shape_view));
	if (gtk_tree_selection_get_selected (selection, NULL, &iter)) {
		char *a;
		gtk_tree_model_get(model,&iter,0,&a,-1);
		vector<Object*>vec=get_all();
		for(Object* shape : vec) {
			if(shape->id==(string)a) {
				unselect_all_shapes();
				block=1;
				shape->vupdate();
				block=0;
				shape->selected=1;
				hide_all();
				shape->drag(0,0,3);
			}
		}
	}

}

void find_str1(GtkTreeModel *model, GtkTreeIter i, string str, string repl, GtkTreeIter parent) {
	while(1) {
		char *a;
		gtk_tree_model_get(model,&i,0,&a,-1);
		if((string)a==str) {
			GtkTreePath* path_parent = gtk_tree_model_get_path(model, &parent);
			gtk_tree_view_expand_row(GTK_TREE_VIEW(shape_view),path_parent,0);
			GtkTreePath* path = gtk_tree_model_get_path(model, &i);
			gtk_tree_store_set(store,&i,0,repl.c_str());
			gtk_tree_view_set_cursor(GTK_TREE_VIEW(shape_view),path,0,0);
			return;
		} else {
			if(gtk_tree_model_iter_has_child(model,&i)) {
				GtkTreeIter child;
				gtk_tree_model_iter_children(model,&child,&i);
				find_str1(model,child,str,repl,i);
			}
		}
		if(!gtk_tree_model_iter_next (model, &i))return;
	}
}
void find_str(GtkTreeModel *model, GtkTreeIter i, string str, GtkTreeIter parent) {
	while(1) {
		char *a;
		gtk_tree_model_get(model,&i,0,&a,-1);
		if((string)a==str) {
			GtkTreePath* path_parent = gtk_tree_model_get_path(model, &parent);
			gtk_tree_view_expand_row(GTK_TREE_VIEW(shape_view),path_parent,0);
			GtkTreePath* path = gtk_tree_model_get_path(model, &i);
			gtk_tree_view_set_cursor(GTK_TREE_VIEW(shape_view),path,0,0);
			return;
		} else {
			if(gtk_tree_model_iter_has_child(model,&i)) {
				GtkTreeIter child;
				gtk_tree_model_iter_children(model,&child,&i);
				find_str(model,child,str,i);
			}
		}
		if(!gtk_tree_model_iter_next (model, &i))return;
	}
}
void change_id(string old,string nw) {
	GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(shape_view));
	GtkTreeIter iter;
	gtk_tree_model_get_iter_first(model,&iter);
	find_str1(model,iter,old,nw,iter);
}
void select_tree_view(string sel_id) {
	GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(shape_view));
	GtkTreeIter iter;
	gtk_tree_model_get_iter_first(model,&iter);
	find_str(model,iter,sel_id,iter);
}
void remove_selected() {
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(shape_view));
	GtkTreeSelection *selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(shape_view));
	if (gtk_tree_selection_get_selected (selection, NULL, &iter)) {
		char *a;
		gtk_tree_model_get(model,&iter,0,&a,-1);
		if(delete_id(a))
			gtk_tree_store_remove(store,&iter);
	}
}
