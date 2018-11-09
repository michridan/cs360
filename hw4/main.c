#include <stdio.h>       // for printf()
#include <stdlib.h>      // for exit()
#include <string.h>      // for strcpy(), strcmp(), etc.
#include <libgen.h>      // for basename(), dirname()
#include <fcntl.h>       // for open(), close(), read(), write()

//for stat syscalls
#include <sys/stat.h>
#include <unistd.h>

// for opendir, readdir syscalls
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("ERROR: too few arguments\n");
		return 0;
	}
	
	return myrcp(argv[1], argv[2]);
}

int myrcp(char *f1, char *f2)
{
	char temp[1024];
	struct stat st1, st2;
	int exists;
	if(lstat(f1, &st1))
	{
		printf("ERROR: %s not found\n", f1);
		return -1;
	}
	else
	{
		if(!S_ISDIR(st1.st_mode) && !S_ISREG(st1.st_mode) && !S_ISLNK(st1.st_mode))
		{
			printf("ERROR: %s is the wrong file type\n", f1);
			return -1;
		}
	}

	if((exists = lstat(f2, &st2) + 1) && !S_ISDIR(st2.st_mode) && !S_ISREG(st2.st_mode) && !S_ISLNK(st2.st_mode))
	{
		printf("ERROR: %s is the wrong file type\n", f2);
		return -1;
	}

	if(!S_ISDIR(st1.st_mode))
	{
    		if(!exists || S_ISREG(st2.st_mode))
				return cpf2f(f1, f2);
			else // f2 exist and is DIR
				return cpf2d(f1,f2);
	}
	else
	{
		
		if (exists && !S_ISDIR(st2.st_mode))
			return -1;
		else if(!exists)
			mkdir(f2, 0755);
		else
		{
			strcpy(temp, f2);
			strcat(temp, "/");
			strcat(temp, basename(f1));
			if(stat(temp, &st2))
				mkdir(temp, 0755);
			else
				return -1;

			if(cpd2d(f1, temp) == -1)
				rmdir(temp);
			return 0;
		}

		return cpd2d(f1, f2);
	}
}

// cp file to file
int cpf2f(char *f1, char *f2)
{
	char buf[4096], *temp;
	struct stat st1, st2;
	int exists, fd1, fd2, n;

	lstat(f1, &st1);
	exists = lstat(f2, &st2) + 1;

	if(exists && st1.st_ino == st2.st_ino)
	{
		printf("ERROR: %s and %s are the same file\n", f1, f2);
		return -1;
	}
	if(S_ISLNK(st1.st_mode))
	{
		if(exists)
		{
			printf("ERROR: %s is a LNK and %s already exists\n", f1, f2);
			return -1;
		}
		else
		{
			buf[readlink(f1, buf, sizeof(buf) - 1)] = '\0';
			symlink(buf, f2);
			return 0;
		}
	}
	fd1 = open(f1, O_RDONLY);
	fd2 = open(f2, O_WRONLY|O_CREAT|O_TRUNC, st1.st_mode);
	
	while(n = read(fd1, buf, st1.st_blksize))
		write(fd2, buf, n);
	return 0;
}

int cpf2d(char *f1, char *f2)
{
	char *base = basename(f1);
	DIR *dp = opendir(f2);
	struct dirent *ep;
	struct stat st2;

	strcat(f2, "/");
	strcat(f2, base);

	//1.search DIR f2 for basename(f1)
	while(ep = readdir(dp))
	{
		if(!strcmp(base, ep->d_name))
		{
			stat(f2, &st2);
			if(S_ISDIR(st2.st_mode))
				return cpf2d(f1, f2);
			else
				return cpf2f(f1, f2);
		}
	}

	return cpf2f(f1, f2);
}

int cpd2d(char *f1, char *f2)
{
	// recursively cp dir into dir    
	char temp1[1024], temp2[1024];
	struct stat st1, stmp, strt;
	DIR *dp = opendir(f1);
	struct dirent *ep;

	stat(f1, &st1);
	stat("/", &strt);
	strcpy(temp2, f2);
	stat(temp2, &stmp);
	// check for a loop 
	while(strt.st_ino != stmp.st_ino)
	{
		if(st1.st_ino == stmp.st_ino)
		{
			printf("ERROR: Can't copy a DIR into itself\n");
			return -1;
		}
		strcat(temp2, "/..");
		stat(temp2, &stmp);
	}

	// then do the stuff
	while(ep = readdir(dp))
	{
		if(strcmp(ep->d_name, ".") && strcmp(ep->d_name, ".."))
		{
			strcpy(temp1, f1);
			strcat(temp1, "/");
			strcat(temp1, ep->d_name);
			strcpy(temp2, f2);
			strcat(temp2, "/");
			strcat(temp2, ep->d_name);
			myrcp(temp1, temp2);
		}
	}
	return 0;
}
