/******** cd_ls_pwd.c ***********/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>

#include "type.h"
//#include "util.c"

/**** globals defined in main.c file ****/

extern MINODE minode[NMINODE];
extern MINODE *root;
extern PROC   proc[NPROC], *running;
extern char gpath[128];
extern char *name[64];
extern int n;
extern int fd, dev;
extern int nblocks, ninodes, bmap, imap, inode_start;
extern char line[256], cmd[32], pathname[256];

int ls_file(int ino)
{
    MINODE *mip = iget(dev, ino);
	INODE stat = mip->INODE;
    // Use INODE contents to ls it 
	char ftime[64], mods[3] = { 'x', 'w', 'r' };
	int i;

	// print mode
	if ((stat.i_mode & 0xF000) == 0x8000) // if (S_ISREG())
		putchar('-');
	else if ((stat.i_mode & 0xF000) == 0x4000) // if (S_ISDIR())
		putchar('d');
	else if ((stat.i_mode & 0xF000) == 0xA000) // if (S_ISLNK())
		putchar('l');

	for (i=8; i >= 0; i--)	
	{
		if (stat.i_mode & (1 << i)) // print r|w|x
			putchar(mods[i % 3]);
		else
			putchar('-');
	}

	// print links, owners, and size
	printf("%4d ", stat.i_links_count);
	printf("%4d ", stat.i_uid);
	printf("%4d ", stat.i_gid);
	printf("%8d ", stat.i_size);

	// print time
	strcpy(ftime, ctime(&stat.i_ctime));
	ftime[strlen(ftime)-1] = 0;
	printf("%s  ",ftime);

	// print name at the DIR level
}

int ls_dir(char *dirname)
{
	char *buf;
    int ino = getino(dirname);
    MINODE mip = iget(dev, ino);
	int i;
	char temp[256];

	for (i=0; i < 12; i++)
	{ 
		if (mip->INODE.i_block[i] == 0)
			break;
		get_block(dev, mip->INODE.i_block[i], dbuf);
	    dp = (DIR *)dbuf;
		cp = dbuf;

	    while (cp < dbuf + BLKSIZE)
		{
		   strncpy(temp, dp->name, dp->name_len);
	       temp[dp->name_len] = 0;

		   ls_file(dp->inode);
		   printf("%s\n", temp);
	       cp += dp->rec_len;
		   dp = (DIR *)cp;
		   
	    }
	}
	return 0;
}

int ls(char *pathname)
{
    int ino = getino(dirname);
    MINODE mip = iget(dev, ino);
	// Check if reg or dir, use correct ls

}

int chdir(char *pathname)
{
	/*
    if (no pathname)
        cd to root;
    else
		cd to pathname by
    (1).  ino = getino(pathname);
    (2).  mip = iget(dev, ino);
    (3).  verify mip->INODE is a DIR
    (4).  iput(running->cwd);
	(5).  running->cwd = mip;
	*/
}

int pwd(MINODE *wd)
{
	if (wd == root)
		printf("/");
    else
         rpwd(wd);
}

int rpwd(MINODE *wd)
{
	/*
	if (wd==root) return;
    //from i_block[0] of wd->INODE: get my_ino of . parent_ino of ..
    pip = iget(dev, parent_ino);
    //from pip->INODE.i_block[0]: get my_name string as LOCAL

    rpwd(pip);  // recursive call rpwd() with pip

    printf("/%s", my_name);
	*/
}
