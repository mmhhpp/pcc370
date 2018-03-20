#include <stdio.h>

#define NFILE 256
static FILE* fileptr[NFILE];

static int initdone=0;

#define POOLSIZE 2097152
static char* mempool;
static char* brkptr;
static char* brklimit;

static fakeinit() {
	if (initdone) return;
	initdone = 1;

	fileptr[0] = stdin;
	fileptr[1] = stdout;
	fileptr[2] = stderr;

	mempool = malloc(POOLSIZE);
	if (!mempool) {
		printf ("mempool malloc failed, exiting\n"); exit(1);
	}
	brkptr = mempool;
	brklimit = mempool + POOLSIZE - 1;
}

int
brk(p) char *p;
{
	fakeinit();
	if( p < mempool ) return -1;
	if( p >= brklimit ) return -1;
	brkptr = p;
	return 0;
}

char *
sbrk(i) int i;
{
	fakeinit();
	if (i == 0) return brkptr;
	if (i < 0) return (char*)-1;
	if (brkptr + i >= brklimit) return (char*)-1;
	brkptr += i;
}

open(pathname, flags) char*pathname; int flags;
{
	int i;
	fakeinit();
	for (i=0; ; i++) {
		if (i >= NFILE) return -1;
		if (!fileptr[i]) break;
	}
	if (flags > 2) flags = 0;
	fileptr[i] = fopen(pathname, (flags == 0) ? "r" : "w");
	if (!fileptr[i])
		return -1;
	return i;
}

creat(pathname, mode) char*pathname; int mode;
{
	return open( pathname, 1);
}

close(fd)
{
	if (fd < 0 || fd > NFILE || !fileptr[fd]) return -1;
	if (fileptr[fd] != stdin && fileptr[fd] != stdout && fileptr[fd] != stderr)
		fclose(fileptr[fd]);
	fileptr[fd] = 0;
	return 0;
}

read(fd, buf, count) int fd; char*buf; int count;
{
	int rc;
	if (fd < 0 || fd > NFILE || !fileptr[fd]) return -1;
	rc = fread(buf, 1, count, fileptr[fd]);
	return rc;
}

write(fd, buf, count) int fd; char*buf; int count;
{
	int rc;
	if (fd < 0 || fd > NFILE || !fileptr[fd]) return -1;
	rc = fwrite(buf, 1, count, fileptr[fd]);
	return rc;
}

long lseek(fd, offset, whence) int fd; long offset; int whence;
{
	int rc;
	if (fd < 0 || fd > NFILE || !fileptr[fd]) return -1;
	if (fileptr[fd] == stdin || fileptr[fd] == stdout || fileptr[fd] == stderr) return -1;
	rc = fseek( fileptr[fd], offset, whence );
	if (rc == -1) return -1;
	return ftell( fileptr[ fd ] );
}

int isatty() { return 1; }

#define FAKE(x,y) x() { printf ("function %d not supported\n",y); return -1; }
FAKE(chdir,1)
FAKE(fork,2)
FAKE(execl,3)
FAKE(seek,5)
FAKE(stat,6)
FAKE(getuid,7)
FAKE(chmod,8)
FAKE(gtty,9)
FAKE(stty,10)
FAKE(getpid,11)
FAKE(time,13)
FAKE(unlink,14)
FAKE(pipe,15)
FAKE(wait,16)
FAKE(dup,17)
FAKE(fstat,18)
FAKE(vrestore,19)
FAKE(nice,20)
FAKE(sleep,21)
FAKE(times,22)
FAKE(random,23)
FAKE(kill,24)
FAKE(execv,25)
FAKE(longjmp,26)
FAKE(localtime,27)
