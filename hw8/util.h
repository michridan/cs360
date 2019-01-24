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
#include <libgen.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX 256

int upload(int sock, char *path);
int download(int sock, char *name, int size);
