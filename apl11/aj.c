#include "apl.h"
#include <string.h>
/* #include <dirent.h> */

double atof();
static char *gettoken();

clear()
{
	struct nlist *n;

	for(n=nlist; n->namep; n++) {
		n->use = 0;
		n->itemp = 0;
		n->namep = 0;
	}
	afreset();	/* release all dynamic memory */
	gsip = 0;	/* reset state indicator */
}

wsload(ffile, port)
{
	char buffer[64], flag, *gettoken(), c;
	int use, size, rank, i, dim[MRANK];
	struct nlist *n;
	struct item *p;

	gettoken(ffile, buffer);
	if (!equal(buffer, "apl\\11")) error("bad ws file magic number");
	gettoken(ffile, buffer);
	gettoken(ffile, buffer);
	thread.fuzz = atof(buffer);
	gettoken(ffile, buffer);
	gettoken(ffile, buffer);
	thread.iorg = atoi(buffer);
	gettoken(ffile, buffer);
	gettoken(ffile, buffer);
	thread.width = atoi(buffer);
	gettoken(ffile, buffer);
	gettoken(ffile, buffer);
	thread.digits = atoi(buffer);

	while (1) {
		if (0 == gettoken(ffile, buffer)) break;
		if (equal(buffer, "DA")) use = DA;
		if (equal(buffer, "CH")) use = CH;
		if (equal(buffer, "NF")) use = NF;
		if (equal(buffer, "MF")) use = MF;
		if (equal(buffer, "DF")) use = DF;

		gettoken(ffile, buffer);
		for (n=nlist; n->namep; n++) {
			if (equal(buffer, n->namep)) {
				erase(n);
				goto hokay;
			}
		}
		n->namep = alloc(1+strlen(buffer));
		strcpy(n->namep, buffer);
hokay:
		n->use = use;
		if (n->use == CH) n->use = DA;
		n->type = LV;

		switch (use) {

			default:
			close(ffile);
			error("bad ws file format");

			case DA:
			case CH:
			rank = atoi(gettoken(ffile, buffer));
			size = 1;
			for (i=0; i<rank; i++) {
				dim[i] = atoi(gettoken(ffile, buffer));
				size *= dim[i];
			}
			p = newdat(use, rank, size);
			for (i=0; i<rank; i++) p->dim[i] = dim[i];
			if (use == CH) read(ffile, (char *)p->datap, size);
			else {
				if (port) {
					data *dp;
					dp = p->datap;
					for (i=0; i<size; i++) {
						*dp++ = atof(gettoken(ffile, buffer));
					}
				}
				else read(ffile, (data *)p->datap, size*sizeof(data));
			}
			n->itemp = p;
			break;

			case NF:
			case MF:
			case DF:
			n->itemp = 0;
			n->label = lseek(wfile, 0L, 2);
			while (1) {
				if (read(ffile, &c, 1) != 1) {
					close(ffile);
					error("wsload unexpected eof");
				}
				write(wfile, &c, 1);
				if (c == 0) break;
			}
			break;
		}
	}
	fdat(ffile);
	close(ffile);
}


static
char *
gettoken(ffile, buffer)
char *buffer;
{
	int i;
	char c;

	i = 0;
	while (1) {
		if (read(ffile, &c, 1) != 1) return 0;
		if (isspace(c)) continue;
		break;
	}
	buffer[i++] = c;
	while (1) {
		if (read(ffile, &c, 1) != 1) break;
		if (isspace(c)) break;
		buffer[i++] = c;
	}
	buffer[i] = 0;
	return buffer;
}


static int portFlag = 0;

wssave(ffile, port)
{
	struct nlist *n;
	int oldPortFlag;

	oldPortFlag = portFlag;
	portFlag = port;
	nsave(ffile, 0);
	for(n=nlist; n->namep; n++) nsave(ffile, n);
	fdat(ffile);
	close(ffile);
	portFlag = oldPortFlag;
}

vsave(fd)
{
	struct nlist *n;
	struct nlist *getnm();

	nsave(fd, 0);
	while(n = getnm()) nsave(fd, n);
	fdat(fd);
	close(fd);
}


nsave(ffile, an)
struct nlist *an;
{
	char c, buffer[64];
	int i, n, size;
	struct item *p;

	if (an == 0){
		write(ffile, "apl\\11\n", 7);
		sprintf(buffer, "fuzz %lg\n", thread.fuzz);
		write(ffile, buffer, strlen(buffer));
		sprintf(buffer, "origin %d\n", thread.iorg);
		write(ffile, buffer, strlen(buffer));
		sprintf(buffer, "width %d\n", thread.width);
		write(ffile, buffer, strlen(buffer));
		sprintf(buffer, "digits %d\n", thread.digits);
		write(ffile, buffer, strlen(buffer));
		return 0;
	}

	if (an->use == 0 || (an->use == DA && an->itemp == 0)) return 0;

	switch (an->use) {
		default:
		close(ffile);
		error("save B");

		case DA:
		p = an->itemp;
		if (p->type == DA) write(ffile, "DA ", 3);
		else write(ffile, "CH ", 3);
		write(ffile, an->namep, strlen(an->namep));
		sprintf(buffer, " %d", p->rank);
		write(ffile, buffer, strlen(buffer));
		size = 1;
		for (i=0; i<p->rank; i++) {
			sprintf(buffer, " %d", p->dim[i]);
			write(ffile, buffer, strlen(buffer));
			size *= p->dim[i];
		}
		write(ffile, "\n", 1);
		if (p->type == DA) {
			data *dp;
			dp = p->datap;
			if (portFlag) {
				for (i=0; i<size; i++) {
					sprintf(buffer, "%14lg\n", *dp++);
					write(ffile, buffer, strlen(buffer));
				}
			}
			else write(ffile, dp, size*sizeof(data));
		}
		else {
			write(ffile, (char *)p->datap, size);
			write(ffile, "\n", 1);
		}
		break;

		case NF:
		write(ffile, "NF ", 3);
		goto real;

		case MF:
		write(ffile, "MF ", 3);
		goto real;

		case DF:
		write(ffile, "DF ", 3);
real:
		write(ffile, an->namep, strlen(an->namep));
		write(ffile, "\n", 1);
		lseek(wfile,(long)an->label,0);
		while (1) {
			read(wfile, &c, 1);
			write(ffile, &c, 1);
			if (c == 0) break;
		}
		break;
	}
	return 0;
}

struct nlist *
getnm()
{
	char *p, name[100];
	struct nlist *n;
	int c;

	while(1){
		printf("variable name? ");
		c = read(1, name, 100);
		if(c <= 1) return(0);
		name[c-1] = 0;
		for(n=nlist; n->namep; n++) if(equal(name, n->namep)) return(n);
		printf("%s does not exist\n", name);
	}
}


listdir()
{
/*
	int f;
	char *p;
	struct direct dir;

	if((f = open(".",0)) < 0) error("directory B");
	while(read(f,&dir,sizeof dir) == sizeof dir) {
		if(dir.d_ino != 0 && dir.d_name[0] != '.') {
			if(column+10 >= thread.width) printf("\n\t");
			for(p=dir.d_name; p<dir.d_name+14 && *p; p++) putchar(*p);
			putchar('\t');
		}
	}
	putchar('\n');
	close(f);
*/

/*
	system("ls .");
*/
#if 0
	DIR *thisDirectory;
	struct dirent *entry;
	int widest, i;

	thisDirectory = opendir(".");
	if (thisDirectory == 0) error("directory B");
	while (1) {
		entry = readdir(thisDirectory);
		if (entry == 0) break;
		if(entry->d_ino != 0 && entry->d_name[0] != '.') {
			if (column+10 >= thread.width) printf("\n\t");
			for (i=0; i<14 && entry->d_name[i]; i++) putchar(entry->d_name[i]);
			putchar('\t');
		}
	}
	putchar('\n');
	closedir(thisDirectory);
#else
	printf ("listdir not supported\n");
#endif

}

fdat(f)
{
	struct stat b;
	struct tm *p, *localtime();

	fstat(f,&b);
	p = localtime(&b.st_mtime);

	printf("  ");
	pr2d(p->tm_hour);
	putchar('.');
	pr2d(p->tm_min);
	putchar('.');
	pr2d(p->tm_sec);
	putchar(' ');
	pr2d(p->tm_mon+1);
	putchar('/');
	pr2d(p->tm_mday);
	putchar('/');
	pr2d(p->tm_year);
}

pr2d(i)
{
	putchar(i/10+'0');
	putchar(i % 10 + '0');
}
