/******** cd_ls_pwd.c ***********/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>

#include "type.h"

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

int ls(char *pathname)
{

}

int chdir(char *pathname)
{

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
	if (wd==root) return;
    //from i_block[0] of wd->INODE: get my_ino of . parent_ino of ..
    pip = iget(dev, parent_ino);
    //from pip->INODE.i_block[0]: get my_name string as LOCAL

    rpwd(pip);  // recursive call rpwd() with pip

    printf("/%s", my_name);
   }

