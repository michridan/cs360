#ifndef FILETREE_H
#define FILETREE_H

#include <stdio.h>
#include <string.h>

typedef struct node
{
	struct node *parent;
	struct node *sibling;
	struct node *child;
	char ntype;
	char name[64];
}NODE;

int insert_node(char ntype, NODE *start);
NODE *find_child(char *name, char ntype, NODE *parent);
int delete_node(char ntype, NODE *start);
NODE *get_node(char ntype, NODE *start);
void print_children(NODE *start);
void path_to_node(NODE *start, char wdpath[]);
void save_to_file(FILE *outfile, NODE *pcur);
void load_from_file(FILE *infile);
void clear(NODE *pcur);

#endif
