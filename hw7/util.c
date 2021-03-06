/****** util.c *****/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>

#include "type.h"
#include "util.h"

/**** globals defined in main.c file ****/

extern MINODE minode[NMINODE];
extern MINODE *root;
extern PROC   proc[NPROC], *running;
extern char gpath[256];
extern char *name[64];
extern int n;
extern int fd, dev;
extern int nblocks, ninodes, bmap, imap, inode_start;
extern char line[256], cmd[32], pathname[256];

/*
int get_block(int dev, int blk, char *buf)
{
	lseek(fd, (long)blk*BLKSIZE, 0);
	read(fd, buf, BLKSIZE);
}   

int put_block(int dev, int blk, char *buf)
{
	lseek(fd, (long)blk*BLKSIZE, 0);
	write(fd, buf, BLKSIZE);
}   
*/
int tokenize1(char *pathname)
{
	int i=0;
  // tokenize pathname into n components: name[0] to name[n-1];
	if(pathname[0] == '/')
		name[i++] = "/";

	name[i++] = strtok(pathname, "/");
	while(name[i++] = strtok(0, "/"));
	return i - 1;
}
/*
MINODE *iget(int dev, int ino)
{
  // return minode pointer to loaded INODE
  (). Search minode[ ] for an existing entry (refCount > 0) with 
       the needed (dev, ino):
       if found: inc its refCount by 1;
                 return pointer to this minode;

  (2). // needed entry not in memory:
       find a FREE minode (refCount = 0); Let mip-> to this minode;
       set its refCount = 1;
       set its dev, ino

  (3). load INODE of (dev, ino) into mip->INODE:
       
       // get INODE of ino a char buf[BLKSIZE]    
       blk    = (ino-1) / 8 + inode_start;
       offset = (ino-1) % 8;

       printf("iget: ino=%d blk=%d offset=%d\n", ino, blk, offset);

       get_block(dev, blk, buf);
       ip = (INODE *)buf + offset;
       mip->INODE = *ip;  // copy INODE to mp->INODE

       return mip;
}


int iput(MINODE *mip) // dispose a used minode by mip
{
 mip->refCount--;
 
 if (mip->refCount > 0) return;
 if (!mip->dirty)       return;
 
 // Write YOUR CODE to write mip->INODE back to disk

} 


// serach a DIRectory INODE for entry with a given name
int search(MINODE *mip, char *name)
{
   // return ino if found; return 0 if NOT
}

*/
// retrun inode number of pathname

int getino1(char *pathname)
{ 
   // SAME as LAB6 program: just return the pathname's ino;
	char temp[256];
	MINODE *mip;
	int n, i, ino;

	strcpy(temp, pathname);
	n = tokenize1(temp);

	if(!strcmp(name[0], "/"))
	{
		i = 1;
		mip = root;
	}
	else
	{
		i = 0;
		mip = running->cwd;
	}

	while(i < n)
	{
		if(!S_ISDIR(mip->INODE.i_mode))
		{
			printf("%s is not a directory!", name[i - 1]);
			return 0;
		}
		ino = search(mip, name[i]);
		if(!ino)
		{
			printf("%s not found\n", name[i]);
			return 0;
		}
		
		mip = iget(dev, ino);
		i++;
	}
	
	return ino;
}



// THESE two functions are for pwd(running->cwd), which prints the absolute
// pathname of CWD. 
/*
int findmyname(MINODE *parent, u32 myino, char *myname) 
{
   // parent -> at a DIR minode, find myname by myino
   // get name string of myino: SAME as search except by myino;
   // copy entry name (string) into myname[ ];
}


int findino(MINODE *mip, u32 *myino) 
{
  // fill myino with ino of . 
  // retrun ino of ..
}
*/
