/************ util.h **********
  This is starting as a collection
  of prelab functions, and I will
  be adding the other utility functions
  here as well, as long as they make
  sense to be here.
 ********************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

void ls_file(char *filename);
void ls_dir(char *dirname);
void ls(char *name);
int find_command(char *cmd);
void execute_command(int cmd, char *path);
int upload(int sock, char *path);
int download(int sock, char *name, int size);
