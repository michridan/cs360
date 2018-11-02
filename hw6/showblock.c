/********* showblock.c code ********/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

#define BLKSIZE 1024

GD    *gp;
SUPER *sp;
INODE *ip;
char dbuf[BLKSIZE], temp[256];
char buf[BLKSIZE];
int fd;
DIR *dp;
char *cp;
int dev;

int search(INODE *inp, char *target)
{
	int i;

	for (i=0; i < 12; i++)
	{  // assume at most 12 direct blocks
		if (inp->i_block[i] == 0)
			break;
		get_block(dev, inp->i_block[i], dbuf);
	    dp = (DIR *)dbuf;
		cp = dbuf;

	    while (cp < dbuf + BLKSIZE)
		{
		   strncpy(temp, dp->name, dp->name_len);
	       temp[dp->name_len] = 0;

		   if(!strcmp(temp, target);
				   return dp->inode;
	       cp += dp->rec_len;
		   dp = (DIR *)cp;
	    }
	}
	return 0;
}

int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
}

int showblock(char *base, char *path)
{
    int ino, blk, offset, i;
    int iblk;
    char ibuf[BLKSIZE];

	get_block(fd, 2, dbuf);
	gp = (GD *)dbuf;

	iblk = gp->bg_inode_table;
	
	get_block(fd, iblock, buf);
	ip = (INODE *)buf + 1;

    for (i=0; i < n; i++)
    {
        ino = search(ip, name[i]);
        if (ino==0)
		{
           printf("can't find %s\n", name[i]); exit(1);
        }
        // Mailman's algorithm: Convert (dev, ino) to inode pointer
        blk    = (ino - 1) / 8 + iblk;  // disk block contain this INODE 
        offset = (ino - 1) % 8;         // offset of INODE in this block
        get_block(dev, blk, ibuf[ ]);
        ip = (INODE *)ibuf + offset;    // ip -> new INODE
    }

	printf("Direct Block Numbers:\n");
	for(i = 0; i < 12; i++)
	{
		if(ip->i_block[i] == 0)
			return 0;

		printf("%d, ", ip->i_block[i]);
		//continue
	}
}

char *disk = "mydisk";
char *name = "/";

int main(int argc, char *argv[ ])
{ 
	char temp[1024], *base, *path;
	if (argc > 2)
	{
		disk = argv[1];
		name = argv[2];
	}
	else if (argc > 1)
		name = argv[1];

	fd = open(disk, O_RDONLY);
	if (fd < 0){
		printf("open failed\n");
		exit(1);
	}

	temp[0] = 0;
	if(strcmp(name, ""))
	{
		strcpy(temp, name);
		base = basename(temp);
		strcpy(temp, name);
		path = pathname(temp);
	}

	showblock();
}
