/************ util.c **********
  This is starting as a collection
  of prelab functions, and I will
  be adding the other utility functions
  here as well, as long as they make
  sense to be here.
 ********************************/
#include "util.h"

int upload(int sock, char *path)
{
	// find file (-1 if not found)

	// check if REG (-1 if not)

	// send file size and name

	// send 256 bits at a time, until size is reached
}

int download(int sock, char *name, int size)
{
	// Pull lines at a time, then write to a file until size is reached
}
