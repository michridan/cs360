/* Name: Filesystem Simulator
 * Version: 0.1
 * By: Michael Dandrea
 * For: CptS 360 Lab 2
 * Last Updated: 9/14/2018
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include "filetree.h"

// Constants
#define N 11
static const char *COMMANDS[N] = {	"quit", "mkdir", "rmdir", "cd", "ls",
									"pwd", "creat", "rm", "save", "reload",
									"menu"	};
// Globals
NODE *root, *cwd;
char line[128];
char command[16], pathname[64];
char dname[64], bname[64];
char *stemp;
char *tokens[32] = {NULL};
int n;

void initialize();
void display_commands(void);
int identify_command(void);
int get_path();
void mkdir(void);	
void rmdir(void);
void cd(void);
void ls(void);
void pwd(void);
void creat(void);
void rm(void);
void save(void);
void reload(void);

int main(void)
{
	int id = 1;
	initialize(); //initialize root node of the file system tree
	printf("Welcome to Filesystem Simulator! Valid commands are:\n");
	display_commands();
	while(id)
	{
		printf("[%s]$ ", cwd->name);
		//get user input line = [command pathname];
		fgets(line, 128, stdin);
		line[strlen(line) - 1] = 0;
		sscanf(line, "%s %s", command, pathname);
		//identify the command;
		id = identify_command();
	    //execute the command;
		if(id)
		{
			switch(id)
			{
				case 1: // mkdir
					mkdir();
					break;
				case 2: // rmdir
					rmdir();
					break;
				case 3: // cd
					cd();
					break;
				case 4: // ls
					ls();
					break;
				case 5: // pwd
					pwd();
					break;
				case 6: // creat
					creat();
					break;
				case 7: // rm
					rm();
					break;
				case 8: // save
					save();
					break;
				case 9: // reload
					reload();
					break;
				case 10: // menu
					display_commands();
					break;
				default:
					break;
			}
			strcpy(pathname, ""); // Reset pathname
		}
		else
		{
			save();
			clear(root);
		}
	}


	return 0;
}

void initialize(void)
{
	root = (NODE *)malloc(sizeof(NODE));
	root->parent = root;
	root->sibling = NULL;
	root->child = NULL;
	root->ntype = 'd';
	strcpy(root->name, "/");
	cwd = root;
}

/*
 * Displays possible commands for the menu function
 */
void display_commands(void)
{
	int i;
	for(i = 0; i < N; i++)
	{
		printf(" %s |", COMMANDS[i]);
	}
	putchar('\n');
}

/*
 * Searches the commands array to find a compatible command
 * returns -1 if not found
 */
int identify_command(void)
{
	int i;

	for(i = 0; i < N; i++)
	{
		if(strcmp(command, COMMANDS[i]) == 0)
			return i;
	}

	return -1;
}

/*
 * Takes the path name, tokenizes it into an array
 * returns the number of steps in the path
 * also seperates dirname and basename
 */
int get_path(void)
{
	char pathcopy[64];
	int i, n = 0;

	strcpy(pathcopy, pathname);
	strcpy(dname, dirname(pathcopy));
	strcpy(pathcopy, pathname);
	strcpy(bname, basename(pathcopy));

	// Clear token array
	for(i = 0; tokens[i]; i++)
		tokens[i] = NULL;

	printf("dirname: %s\tbasename:%s\n", dname, bname);

	return n;
}


void mkdir(void)
{
	if(pathname[0] == '/')
	{
		if(insert_node('d', root))
			printf("mkdir SUCCESSFUL\n");
		else
			printf("mkdir FAILED\n");
	}
	else
	{
		if(insert_node('d', cwd))
			printf("mkdir SUCCESSFUL\n");
		else
			printf("mkdir FAILED\n");
	}
}

void rmdir(void)
{
	if(pathname[0] == '/')
	{
		if(delete_node('d', root))
			printf("rmdir SUCCESSFUL\n");
		else
			printf("rmdir FAILED\n");
	}
	else
	{
		if(delete_node('d', cwd))
			printf("rmdir SUCCESSFUL\n");
		else
			printf("rmdir FAILED\n");
	}
}

void cd(void)
{
	NODE *ntemp;

	if(!strcmp(pathname, ""))
	{
		cwd = root;
		printf("cd SUCCESSFUL\n");
	}
	else if(pathname[0] == '/')
	{
		ntemp = get_node('d', root);
		if(ntemp)
		{
			cwd = ntemp;
			printf("cd SUCCESSFUL\n");
		}
		else
			printf("cd FAILED\n");
	}
	else
	{
		ntemp = get_node('d', cwd);
		if(ntemp)
		{
			cwd = ntemp;
			printf("cd SUCCESSFUL\n");
		}
		else
			printf("cd FAILED\n");
	}
}

void ls(void)
{
	NODE *ntemp;

	if(!strcmp(pathname, ""))
	{
		print_children(cwd);
	}
	else if(pathname[0] == '/')
	{
		ntemp = get_node('d', root);
		if(ntemp)
		{
			print_children(cwd);
		}
	}
	else
	{
		ntemp = get_node('d', cwd);
		if(ntemp)
		{
			print_children(cwd);
		}
	}
}

void pwd(void)
{
	char path[64];
	path_to_node(cwd, path);
	printf("%s\n", path);
}

void creat(void)
{
	if(pathname[0] == '/')
	{
		if(insert_node('f', root))
			printf("creat SUCCESSFUL\n");
		else
			printf("creat FAILED\n");
	}
	else
	{
		if(insert_node('f', cwd))
			printf("creat SUCCESSFUL\n");
		else
			printf("creat FAILED\n");
	}
}

void rm(void)
{
	if(pathname[0] == '/')
	{
		if(delete_node('f', root))
			printf("rm SUCCESSFUL\n");
		else
			printf("rm FAILED\n");
	}
	else
	{
		if(delete_node('f', cwd))
			printf("rm SUCCESSFUL\n");
		else
			printf("rm FAILED\n");
	}
}

void save(void)
{
	FILE *outfile = fopen("savefile", "w");

	if(outfile)
	{
		save_to_file(outfile, root->child);
		printf("save SUCCESSFUL\n");
		fclose(outfile);
	}
	else
		printf("save FAILED\n");
}

void reload(void)
{
	FILE *infile = fopen("savefile", "r");

	if(infile)
	{
		clear(root->child);
		root->child = NULL;
		load_from_file(infile);
		printf("load SUCCESSFUL\n");
		fclose(infile);
	}
	else
		printf("load FAILED\n");
}

// Tree-specific functions

/*
 * inserts a new node at the given location
 * returns 1 if successful, 0 otherwise
 */
int insert_node(char ntype, NODE *start)
{
	char dircopy[64];
	int success = 0, i = 0;
	NODE *newnode = NULL, *pcur = start, *pprev;

	get_path();
	n = 0;

	strcpy(dircopy, dname);
	stemp = strtok(dircopy, "/"); // first call to strtok()
	while(stemp)
	{
		tokens[n] = stemp;
		n++;
		stemp = strtok(0, "/"); // call strtok() until it returns NULL
	}

	while(tokens[i] && pcur && strcmp(tokens[i], "."))
	{
		printf("searching for %s in %s...", tokens[i], pcur->name);
		
		pprev = pcur;
		pcur = find_child(tokens[i++], 'd', pprev);
	}

	if(pcur)
	{
		printf("searching for %s in %s...", bname, pcur->name);
		pprev = pcur;
		if(!find_child(bname, 'd', pprev) && !find_child(bname, 'f', pprev))
		{
			success = 1;
			newnode = (NODE *)malloc(sizeof(NODE));
			newnode->sibling = NULL;
			newnode->child = NULL;
			strcpy(newnode->name, bname);
			newnode->ntype = ntype;
			pcur = pcur->child;
			if(!pcur)
			{
				pprev->child = newnode;
				newnode->parent = pprev;
			}
			else
			{
				while(pcur)
				{
					pprev = pcur;
					pcur = pcur->sibling;
				}
				pprev->sibling = newnode;
				newnode->parent = pprev->parent;
			}
		}
		else
			printf("ERROR: %s already exists!\n", bname);
	}

	return success;
}

int delete_node(char ntype, NODE *start)
{
	char dircopy[64];
	int success = 0, i = 0;
	NODE *newnode = NULL, *pcur = start, *pprev;

	get_path();
	n = 0;

	strcpy(dircopy, dname);
	stemp = strtok(dircopy, "/"); // first call to strtok()
	while(stemp)
	{
		tokens[n] = stemp;
		n++;
		stemp = strtok(0, "/"); // call strtok() until it returns NULL
	}

	i = 0;
	while(tokens[i] && pcur && strcmp(tokens[i], "."))
	{
		printf("searching for %s in %s...", tokens[i], pcur->name);
		
		pprev = pcur;
		pcur = find_child(tokens[i++], 'd', pprev);
	}

	if(pcur)
	{
		printf("searching for %s in %s...", bname, pcur->name);
		pprev = pcur;
		if(find_child(bname, ntype, pprev))
		{
			pcur = pcur->child;

			if(!strcmp(bname, pcur->name) && ntype == pcur->ntype)
			{
				if(pcur->child)
				{
					printf("ERROR: Can't delete a nonempty directory!\n");
					return 0;
				}
				else if(pcur == cwd)
				{
					printf("ERROR: Can't delete current working directory!\n");
					return 0;
				}
					pprev->child = pcur->sibling;
					success = 1;
			}
			else
			{
				while(!success)
				{
					if(!strcmp(bname, pcur->name) && ntype == pcur->ntype)
					{
						if(pcur->child)
						{
							printf("ERROR: Can't delete a nonempty directory!\n");
							return 0;
						}
						else if(pcur == cwd)
						{
							printf("ERROR: Can't delete current working directory!\n");
							return 0;
						}
						success = 1;
						pprev->sibling = pcur->sibling;
					}
					if(!success)
					{
						pprev = pcur;
						pcur = pcur->sibling;
					}
				}
			}
			free(pcur);
			pcur = NULL;
		}
		else
			printf("ERROR: %s doesn't exist!\n", bname);
	}

	return success;
}

NODE *find_child(char *name, char ntype, NODE *parent)
{
	NODE *pcur = parent->child;
	
	while(pcur)
	{
		if(!strcmp(name, pcur->name) && ntype == pcur->ntype)
		{
			printf("found\n");
			return pcur;
		}
		
		pcur = pcur->sibling;
	}
	printf("not found\n");
	return pcur;
}

NODE *get_node(char ntype, NODE *start)
{
	char dircopy[64];
	int success = 0, i = 0;
	NODE *newnode = NULL, *pcur = start, *pprev;

	get_path();
	n = 0;

	strcpy(dircopy, dname);
	stemp = strtok(dircopy, "/"); // first call to strtok()
	while(stemp)
	{
		tokens[n] = stemp;
		n++;
		stemp = strtok(0, "/"); // call strtok() until it returns NULL
	}

	if(!n && !strcmp(bname, ".."))
		return cwd->parent;
	else if(!strcmp(bname, "/"))
		return root;

	i = 0;
	while(tokens[i] && pcur && strcmp(tokens[i], "."))
	{
		printf("searching for %s in %s...", tokens[i], pcur->name);
		
		pcur = find_child(tokens[i++], 'd', pcur);
	}

	if(pcur)
	{
		printf("searching for %s in %s...", bname, pcur->name);

		pcur = find_child(bname, 'd', pcur);
	}

	return pcur;
}

void print_children(NODE *start)
{
	NODE *pcur = start->child;

	while(pcur)
	{
		printf("%c\t%s\n", pcur->ntype, pcur->name);
		pcur = pcur->sibling;
	}
}

void path_to_node(NODE *pcur, char wdpath[])
{	
	if(pcur == pcur->parent) // Meaning we have hit the root
	{
		strcpy(wdpath, pcur->name);
	}
	else
	{
		path_to_node(pcur->parent, wdpath);
		strcat(wdpath, pcur->name);
		if(pcur != cwd)
			strcat(wdpath, "/"); 
	}
}

void save_to_file(FILE *outfile, NODE *pcur)
{
	char path[64];
	if(pcur)
	{
		path_to_node(pcur, path);
		path[strlen(path) - 1] = 0;
		fprintf(outfile, "%c %s\n", pcur->ntype, path);
		save_to_file(outfile, pcur->child);
		save_to_file(outfile, pcur->sibling);
	}
}

void load_from_file(FILE *infile)
{
	char ntype[3];

	while(!feof(infile))
	{
		fgets(line, 128, infile);
		sscanf(line, "%s %s", ntype, pathname);
		if(ntype[0] == 'd')
		{
			mkdir();
		}
		else if(ntype[0] == 'f')
		{
			creat();
		}
		strcpy(pathname, "");
	}
}

void clear(NODE *pcur)
{
	if(pcur)
	{
		clear(pcur->child);
		clear(pcur->sibling);
		free(pcur);
	}
}
