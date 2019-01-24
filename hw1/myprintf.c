#include "myprintf.h"

char *ctable = "0123456789ABCDEF";
int  BASE = 10; 

int rpu(u32 x)
{  
    char c;
	if (x)
	{
		c = ctable[x % BASE];
        rpu(x / BASE);
        putchar(c);
    }
}

int printu(u32 x)
{
   (x==0)? putchar('0') : rpu(x);
   putchar(' ');
}

int prints(char *s)
{
	int i;

	for(i = 0; s[i] != '\0'; i++)
	{
		putchar(s[i]);
	}
}

int printd(int x)
{
	if(x == 0)
		putchar('0');
	else if(x < 0)
	{
		putchar('-');
		rpu(0 - x);
	}
	else
		rpu(x);

	putchar(' ');
}

int printx(u32 x)
{
	BASE = 16;

	putchar('0');
	putchar('x');

	(x==0)? putchar('0') : rpu(x);
	putchar(' ');

	BASE = 10;
}

int printo(u32 x)
{
	BASE = 8;

	putchar('0');

	(x==0)? putchar('0') : rpu(x);
	putchar(' ');

	BASE = 10;
}

int myprintf(char *fmt, ...)
{
	va_list list;
	int i = 0;

	va_start(list, fmt);

	while(fmt[i] != '\0')
	{
		if(fmt[i] == '%')
		{
			i++;

			switch(fmt[i])
			{
			case 'c': putchar(va_arg(list, int));
				break;
			case 's': prints(va_arg(list, char *));
				break;
			case 'u': printu(va_arg(list, u32));
				break;
			case 'd': printd(va_arg(list, int));
				break;
			case 'o': printu(va_arg(list, u32));
				break;
			case 'x': printu(va_arg(list, u32));
				break;
			default:
				break;
			}

			i++;
		}
		else
			putchar(fmt[i++]);
	}

//	The code below is how we were advised to find the parameters, however, I wasn't
//	able to get it working, so I'm leaving it commented out down here
//
//	char *cp = fmt;
//	int i = 0, *ip = (int *)(cp + 4);
//
//	while(cp[i] != '\0')
//	{
//		if(cp[i] == '%')
//		{
//			i++;
//			switch(cp[i])
//			{
//			case 'c': putchar((char)*ip);
//				break;
//			case 's': prints((char *)*ip);
//				break;
//			case 'u': printu((u32)*ip);
//				break;
//			case 'd': printd(*ip);
//				break;
//			case 'o': printo((u32)*ip);
//				break;
//			case 'x': printx((u32)*ip);
//				break;
//			default:
//				break;
//			}
//
//			ip++;
//			i++;
//		}
//		else
//			putchar(cp[i++]);
//	}
}
