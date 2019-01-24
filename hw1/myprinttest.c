#include "myprintf.h"

int main(void)
{
	printf("Testing myprintf\n\n");

	printf("Expected output:\tTesting one two three\n");
	printf("Actual output:\t");
	myprintf("Testing one two three\n");
	putchar('\n');

	myprintf("%u\n", 25);

	printf("Expected output:\t%c %s %u %d %o %x\n", 'a', "test", 300, -300, 300, 300);
	printf("Actual output:\t");
	myprintf("%c %s %u %d %o %x\n", 'a', "test", 300, -300, 300, 300);

	return 0;
}
