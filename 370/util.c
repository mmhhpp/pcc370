#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int
is_blank(s)
char *s;
{
	char *p;
	if (*s == 0)
		return 1;
	for (p=s; *p; p++) {
		if (*p == '\n') continue;
		if (*p == '\t') continue;
		if (*p == ' ') continue;
		return 0;
	}
	return 1;
}

int main(argc, argv)
int argc;
char**argv;
{
	char buffer[1024];
	char * cp1;
	int i;
	int do_ucase = 0;

	if (argc > 1 && argv[1][0] == 'U')
		do_ucase = 1;

	while ( (cp1 = fgets(buffer, sizeof(buffer), stdin) ) ) {
		if (is_blank(buffer))
			continue;
		if (do_ucase) for (i=0; i<strlen(buffer); i++) {	
			buffer[i] = (islower(buffer[i])) ? toupper(buffer[i]) : buffer[i];
		}
		fputs(buffer, stdout);
	}
	return (0);
}
