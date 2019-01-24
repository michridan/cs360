#ifndef MYPRINTF_H
#define MYPRINTF_H

#include <stdio.h>
#include <stdarg.h>

typedef unsigned int u32;

extern char *ctable;
extern int  BASE; 

int rpu(u32 x);
int printu(u32 x);
int prints(char *s);
int printd(int x);
int printx(u32 x);
int printo(u32 x);
int myprintf(char *fmt, ...);

#endif
