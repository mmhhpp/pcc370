#include "apl.h"
#include <signal.h>

char *iofname();


/*
 * misc. other routines
 */

ex_exit()
{
	term(topfix());
}

ex_signl()
{
	int i,j;

	i = topfix();
	j = topfix() != 0;
	iodone((int)signal(i,(int (*)())j));
}

ex_fork()
{
	int pid;
	struct item *p;

	if ((pid = fork(0)) == -1) error("couldn't fork");
	pop();
	iodone(pid);
}

ex_wait()
{
	struct item *p;
	int (*sig)(), pid;
	int s;

	sig = signal(SIGINT, SIG_IGN);
	pid = wait(&s);
	signal(SIGINT, sig);
	p = newdat(DA, 1, 3);
	p->datap[0] = pid;
	p->datap[1] = s&0377;
	p->datap[2] = (s>>8)&0377;
	pop();		/* dummy arg */
	*sp++ = p;
}

#define MAXP 20

ex_exec()
{
	struct item *p;
	int i, j;
	char *cp, *argv[MAXP+1];

	p = fetch1();
	if (!p->rank || p->rank > 2 || p->size > 500 || p->type != CH) error("Lexec D");
	if (p->rank == 2){
		if (p->dim[0] > MAXP) error("Lexec D");
		cp = (char *)(p->datap);
		for(i=0; i<p->dim[0]; i++) argv[i] = cp + i*p->dim[1];
		argv[p->dim[0]] = 0;
	}
	else {
		cp = (char *)(p->datap);
		for(i=j=0; i < MAXP && cp < (char *)(p->datap)+p->size; cp++) {
			if (!*cp) j = 0;
			else if (!j){
				j = 1;
				argv[i++] = (char *)cp;
			}
		}
		if (i == MAXP || *--cp) error("Lexec D");
		argv[i] = 0;
	}
	execv(argv[0], &argv[1]);
	pop();
	p = newdat(DA,0,0);
	*sp++ = p;
}

ex_chdir()
{
	iodone(chdir(iofname()));
}

ex_write()
{
	int fd, m;
	struct item *p;
	int mult;			/* Multiplier (data size) */

	fd = topfix();
	p = fetch1();
	if(p->type != CH && p->type != DA) error("Lwrite D");
	mult = p->type == CH ? 1 : sizeof datum;
	m = write(fd, p->datap, p->size * mult) / mult;
	pop();
	iodone(m);
}

ex_creat()
{
	int m;

	m = topfix();
	iodone(creat(iofname(), m));
}

ex_open()
{
	struct item *p;
	int m;

	m = topfix();
	iodone(open(iofname(), m));
}

ex_seek()
{
	struct item *p;
	int k1, k3;
	long k2;

	p = fetch1();
	if(p->type != DA || p->rank != 1 || p->size != 3) error("Lseek D");
	k1 = p->datap[0];
	k2 = p->datap[1];
	k3 = p->datap[2];
	k1 = lseek(k1, k2, k3);
	pop();
	iodone(k1);
}

ex_close()
{
	iodone(close(topfix()));
}

ex_pipe()
{
	struct item *p;
	int pp[2];

	if(pipe(pp) == -1) p = newdat(DA, 1, 0);
	else {
		p = newdat(DA, 1, 2);
		p->datap[0] = pp[0];
		p->datap[1] = pp[1];
	}
	pop();
	*sp++ = p;
}

ex_read()
{
	struct item *p, *q;
	int fd, nb, c;

	fd = topfix();
	nb = topfix();
	p = newdat(CH, 1, nb);
	c = read(fd, p->datap, nb);
	if(c != nb){
		q = p;
		if(c <= 0) p = newdat(CH, 1, 0);
		else {
			p = newdat(CH, 1, c);
			copy(CH, q->datap, p->datap, c);
		}
		dealloc(q);
	}
	*sp++ = p;
}

ex_unlink()
{
	iodone(unlink(iofname()));
}

ex_kill()
{
	int pid, signo;

	pid = topfix();
	signo = topfix();
	kill(pid, signo);
	*sp++ = newdat(DA, 1, 0);
}

ex_rd()
{
	/*
	 * note:
	 * an empty line is converted to NULL.
	 * no '\n' chars are returned.
	 */
	char buf[200];
	struct item *p;
	int fd, i;

	fd = topfix();
	i = 0;
	while((read(fd, &buf[i], 1) == 1) && i < 200 && buf[i] != '\n') i++;
	if(i == 200) error("Lrd D");
	if(i > 0){
		p = newdat(CH, 1, i);
		copy(CH, buf, p->datap, i);
	}
	else p = newdat(CH, 1, 0);
	*sp++ = p;
}

ex_dup()
{
	iodone(dup(topfix()));
}

ex_ap()
{
	int i, fd;
	struct item *p;

	fd = topfix();
	p = fetch1();
	lseek(fd, 0L, 2);
	fappend(fd, p);
	if(p->rank == 1) write(fd, "\n", 1);
	pop();
	*sp++ = newdat(DA, 1, 0);
}

ex_float()
{

	/* Convert characters into either double-precision (apl)
	 * or single-precision (apl2) format.  (Involves only
	 * changing the data type and size declarations.
	 */

	struct item *p;
	int tmpi;

	p = fetch1();									/* Get variable descriptor */
	if (p->type != CH) error("topval C");			/* Must be characters */
	if (p->rank == 0 								/* Scalar */
		|| p->dim[(p->rank) - 1] % sizeof datum)	/* Bad size */
			error("float D");
	tmpi = p->dim[p->rank - 1];			/* Reduce dimensions */
	tmpi = tmpi / sizeof datum;
	p->dim[p->rank - 1] = tmpi;
	tmpi = p->size;						/* Reduce size */
	tmpi = tmpi / sizeof datum;
	p->size = tmpi;
	p->type = DA;									/* Change data type */
}

iodone(ok)
{
	struct item *p;

	p = newdat(DA, 0, 1);
	p->datap[0] = ok;
	*sp++ = p;
}

char *
iofname(m)
{
	struct item *p;
	char b[200];

	p = fetch1();
	if(p->type != CH || p->rank > 1) error("file name D");
	copy(CH, p->datap, b, p->size);
	b[p->size] = 0;
	pop();
	return(b);
}

fappend(fd, ap)
struct item *ap;
{
	struct item *p;
	char *p1;
	int i, dim0, dim1, sb[32];
	char b[200];

	p = ap;
	if((p->rank != 2 && p->rank != 1) || p->type != CH) error("file append D");
	dim1 = p->dim[1];
	dim0 = p->dim[0];
	if(p->rank == 1) dim1 = dim0;
	p1 = (char *)(p->datap);
	if(p->rank == 2) {
		for(i=0; i<dim0; i++){
			copy(CH, p1, b, dim1);
			p1 += dim1;
			b[ dim1 ] = '\n';
			write(fd, b, dim1+1);
		}
	}
	else write(fd, p->datap, dim0);
}
