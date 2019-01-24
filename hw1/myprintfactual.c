#include "myprintf.h"

int main(int argc, char *argv[], char *env[])
{
	int i;
	printf("Using myprintf from here on out:\n");

	myprintf("argc = %d\n", argc);
	
	for(i = 0; i < argc; i++)
	{
		myprintf("argv[%d] = %s\n", i, argv[i]);
	}

	for(i = 0; env[i]; i++)
	{
		myprintf("env[%d] = %s\n", i, env[i]);
	}

	return 0;
}
