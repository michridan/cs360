/************* t.c file ********************/
#include <stdio.h>
#include <stdlib.h>

int *FP;
int A(int x, int y);
int B(int x, int y);
int C(int x, int y);
int *getebp(void);

int main(int argc, char *argv[ ], char *env[ ])
{
	int a,b,c,i;
	printf("enter main\n");
		  
	printf("&argc=%x argv=%x env=%x\n", (unsigned int)&argc, (unsigned int)argv, (unsigned int)env);
	printf("&a=%8x &b=%8x &c=%8x\n", (unsigned int)&a, (unsigned int)&b, (unsigned int)&c);

//	(1). Write C code to print values of argc and argv[] entries

	printf("argc=%d\n", argc);

	for(i = 0; i < argc; i++)
	{
		printf("argv[%d]=%s\n", i, argv[i]);
	}

	a=1; b=2; c=3;
	A(a,b);
	printf("exit main\n");
}

int A(int x, int y)
{
	int d,e,f;
	printf("enter A\n");
	// PRINT ADDRESS OF d, e, f
	printf("&d=%8x &e=%8x &f=%8x\n", (unsigned int)&d, (unsigned int)&e, (unsigned int)&f);
	d=4; e=5; f=6;
	B(d,e);
	printf("exit A\n");
}

int B(int x, int y)
{
	int g,h,i;
	printf("enter B\n");
	// PRINT ADDRESS OF g,h,i
	printf("&g=%8x &h=%8x &i=%8x\n", (unsigned int)&g, (unsigned int)&h, (unsigned int)&i);
	g=7; h=8; i=9;
	C(g,h);
	printf("exit B\n");
}

int C(int x, int y)
{
	int u, v, w, i, *p, index;

	printf("enter C\n");
	// PRINT ADDRESS OF u,v,w,i,p;
	printf("&u=%8x &v=%8x &w=%8x &i=%8x &p=%8x\n", (unsigned int)&u, (unsigned int)&v, (unsigned int)&w, (unsigned int)&i, (unsigned int)&p);
	u=10; v=11; w=12; i=13;

	FP = (int *)getebp();

//  (2). Write C code to print the stack frame link list.

	printf("Printing stack frame linked list\n");
	printf("%8x -> ", (unsigned int)FP);
	FP = (int *)*FP;
	printf("%8x -> ", (unsigned int)FP);
	FP = (int *)*FP;
	printf("%8x -> ", (unsigned int)FP);
	FP = (int *)*FP;
	printf("%8x ->", (unsigned int)FP);
	FP = (int *)*FP;
	printf("%8x ->", (unsigned int)FP);
	FP = (int *)*FP;
	printf("%8x\n", (unsigned int)FP);

	p = (int *)&p;

//  (3). Print the stack contents from p to the frame of main()
//       YOU MAY JUST PRINT 128 entries of the stack contents.
	
	printf("Printing stack contents, starting at p\n");
	for(index = 0; index < 128 && p >= 0; index++, p++)
	{
		printf("%8x\t=\t%8x\n", (unsigned int)p, (unsigned int)*p);
	}
//  (4). On a hard copy of the print out, identify the stack contents
//       as LOCAL VARIABLES, PARAMETERS, stack frame pointer of each function.
}
