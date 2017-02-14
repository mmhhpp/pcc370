
#include <stdio.h>

int main(argc, argv) int argc; char *argv[];
{
	int i1;
	for (i1 = 1; i1 < argc; i1++) {
		printf ("Hello world arg[%d] = %s\n", argc, argv[i1]);
	}
}
