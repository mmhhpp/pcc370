#include <signal.h>
#include "apl.h"

extern int	chartab[];
int mkcore  =   1;      /* if "1", produce core image upon fatal error */
int edmagic  =  0;      /* if "1", turn on "ed" magic characters */
int normalExit = 0;		/* set to true when doing a normal exit */

char headline[] = "a p l \\ 1 1\n12 nov 1992\n\n";

#ifndef ibm
void reset() { printf ("reset() not implemented, exiting\n");  exit(1); }
void setexit() {}
#endif

main(argc, argp)
char **argp;
{
	char *p;
	int a, b, c;
	static int fflag;
	int intr(), intprws();
	time(&startTime);
	/*
	 * setup scratch files 
	 */
	a = getpid();
	scr_file = malloc(32);
	ws_file = malloc(32);
	sprintf(scr_file, "/tmp/apled.%d", a);
	strcpy(ws_file, "dd:tmpws");

	offexit = isatty(0);
	echoflg = !offexit;
	a = 1;

	/* Check to see if argp[0] is "prws".  If so, set prwsflg */

	for(p=argp[0]; *p; p++);
	while(p > argp[0] && *p != '/') p--;
	if (*p == '/') p++;
	for(c=0; c < 4; c++) {
		if (!p[c] || p[c] != "prws"[c]) goto notprws;
	}
	prwsflg = 1;
	close(0);

notprws:

	/* other flags... */

	apl_term = 0;

	while(argc > 1 && argp[1][0] == '-'){
		argc--;
		argp++;
		while(*++*argp) switch(**argp){
		case 'e':	echoflg = 1;	break;
		case 'q':	echoflg = 0;	break;
		case 'd':
		case 'D':	a = 0;
		case 'c':
		case 'C':	mkcore = 1;	break;
		case 't':	scr_file  += 5;
					ws_file += 5;
		case 'm':	apl_term = 1;	break;
		case 'r':	edmagic = 1;	break;
		case 'o':	offexit = 0;	break;
		}
	}

	if (prwsflg) echoflg = mkcore = a = 0;	/* "prws" settings */

	thread.iorg = 1;
	thread.width = 72;
	thread.digits = 9;
	thread.fuzz = 1.0e-13;

	aplmod(1);								/*	Turn on APL mode */
	if (a) catchsigs();						/*	Catch signals  */
	if (prwsflg) signal(SIGINT, intprws);
	else fppinit();

	/*
	 * open ws file
	 */

	/* close(opn(WSFILE,0600)); */
	wfile = opn(WSFILE,2);
	zero = 0;
	one = 1;
	maxexp = 88;
	pi = 3.141592653589793238462643383;

	sp = stack;
	fflag = 1;
	mencflg = 0;
	ifile = 0;
	if (!prwsflg) {
		if (signal(SIGINT, intr) == -1) signal(SIGINT, SIG_IGN);
		printf(headline);
	}
	setexit();
	if(fflag) {
		fflag = 0;
		if(argc > 1 && (a = opn(argp[1], 0)) > 0){
			wsload(a, 0);
			printf(" %s\n", argp[1]);
			close(a);
		}
		else {
			if((a=open("continue",0)) < 0) printf("clear ws\n");
			else {
				wsload(a, 0);
				printf(" continue\n");
				close(a);
			}
		}
		if (prwsflg){
			ex_prws();
			term(0);
		}
		evLlx();					/* eval latent expr, if any */
	}
	/* setjmp(reset_env); */
	mainloop();
}

mainloop()
{
	char *a, *comp;
	static eotcount = MAXEOT;		/* maximum eot's on input */

	comp = 0;
	setexit();
	while(1){
		if (comp) {
			aplfree(comp);
			comp = 0;
		}
		lineNumber = -1;
		if(echoflg) echoflg = 1;	/* enabled echo echo suppress off */
		checksp();
		if(intflg) error("I");
		putchar('\t');
		a = rline(8);
		if(a == 0) {
			offexit &= isatty(0);
			if (offexit) {
				if (eotcount-- > 0) printf("\ruse \')off\' to exit\n");
				else panic(0);
				continue;
			}
			else term(0);			/* close down and exit */
		}
		comp = compile(a, 0);
		aplfree(a);
		if(comp == 0) continue;
		execute(comp);
		/* note that if the execute errors out, then
		 * the allocated space pointed to by comp is never
		 * freed.  This is hard to fix.
		 */
	}
}

intr()
{
	intflg = 1;
	signal(SIGINT, intr);
	lseek(0, 0L, 2);
}

intprws()
{
	/* "prws" interrupt -- restore old tty modes and exit */

	term(0177);
}

char *
rline(s)
{
	int rlcmp();
	char line[CANBS];
	char *p;
	int c, col;
	char *cp, *retval;
	char *dp;
	int i,j;

	column = 0;
	col = s;
	p = line;
loop:
	c = getchar();
	if(intflg) error("I");
	switch(c) {

	case '\0':
	case -1:
		return(0);

	case '\b':
		if(col) col--;
		goto loop;

	case '\t':
		col = (col+8) & ~7;
		goto loop;

	case ' ':
		col++;
		goto loop;

	case '\r':
		col = 0;
		goto loop;

	default:
		*p++ = col;
		*p++ = c;					/* was and'ed with 0177... */
		col++;
		goto loop;

	case '\n':
		;
	}
	qsort(line, (p-line)/2, 2, rlcmp);
	c = p[-2];
	if(p == line) c = 1;			/* check for blank line */
	*p = -1;
	col = -1;
	cp = (retval=alloc(c+3)) - 1;
	for(p=line; p[0] != -1; p+=2) {
		while(++col != p[0]) *++cp = ' ';
		*++cp = p[1];
		while(p[2] == col) {
			if(p[3] != *cp) {
				i = *cp ;
				*cp = p[3];
				break;
			}
			p += 2;
		}
		if(p[2] != col)	continue;
		while(p[2] == col) {
			if(p[3] != *cp) goto yuck;
			p += 2;
		}
		i = ((i<<8) | *cp)&0177777;
		for(j=0; chartab[j]; j++){
			if(i == chartab[j]) {
				*cp = j | 0200;
				j = 0;
				break;
			}
		}
		if(j) {
yuck:
			*cp = '\n';
			pline(cp, ++col, -1);
			aplfree(retval);			/*	Free dynamic space  */
			error("Y error");
		}
	}
	*++cp = '\n';
	return(retval);
}

rlcmp(a, b)
char *a, *b;
{
	int c;

	if(c = a[0] - b[0]) return(c);
	return(a[1] - b[1]);
}

pline(str, loc, ln)
char *str;
{
	int c, l, col;

	col = 0;
	l = 0;
	if (ln >= 0) {
		printf("[%d]\t", ln-1);
		loc += 8;
		l += 8;
	}
	do {
		c = *str++;
		l++;
		if(l == loc) col = column;
		putchar(c);
	} while(c != '\n');
	if(col) {
		putto(col);
		putchar('^');
		putchar('\n');
	}
}

putto(col)
{
	while(col > column+8) putchar('\t');
	while(col > column) putchar(' ');
}

term(s)
{

	int j;

	for(j=3; j<NFDS; j++) close(j);		/* Close files */
	unlink(WSFILE);						/* Unlink temporaries */
	unlink(scr_file);
	aplmod(0);							/* Turn off APL mode */
	normalExit = 1;						/* Set this flag */
	exit(s);							/* And we're outa here */
}

fix(d)
data d;
{
	int i;

	i = floor(d+0.5);
	return(i);
}

fuzz(d1, d2)
data d1, d2;
{
	double f1, f2;

	f1 = d1;
	if(f1 < 0.) f1 = -f1;
	f2 = d2;
	if(f2 < 0.) f2 = -f2;
	if(f2 > f1) f1 = f2;
	f1 *= thread.fuzz;
	if(d1 > d2) {
		if(d2+f1 >= d1) return(0);
		return(1);
	}
	if(d1+f1 >= d2) return(0);
	return(-1);
}

pop()
{
	if(sp <= stack) error("pop B");
	if (sp[-1]) dealloc(sp[-1]);
	sp--;
}

erase(np)
struct nlist *np;
{
	int *p;

	p = np->itemp;
	if(p) {
		switch(np->use) {
		case NF:
		case MF:
		case DF:
			for(; *p>0; (*p)--) aplfree(p[*p]);

		}
		aplfree(p);
		np->itemp = 0;
	}
	np->use = 0;
}

dealloc(p)
struct item *p;
{
	if (p == 0) {
		printf("null pointer passed to dealloc\n");
		return 0;
	}

	switch(p->type) {
	default:
		printf("[dealloc botch: %d]\n", p->type);
		return 0;
	case LBL:
		((struct nlist *)p)->use = 0;     /* delete label */
	case LV:
		return 0;

	case DA:
	case CH:
	case QQ:
	case QD:
	case QC:
	case EL:
	case DU:
	case QX:
		aplfree(p);
	}
	return 0;
}

struct item *
newdat(type, rank, size)
{
	int i;
	struct item *p;

	/* Allocate a new data item.  I have searched the specifications
	 * for C and as far as I can tell, it should be legal to
	 * declare a zero-length array inside a structure.  However,
	 * the VAX C compiler (which I think is a derivative of the
	 * portable C compiler) does not allow this.  The Ritchie
	 * V7 PDP-11 compiler does.  I have redeclared "dim" to
	 * contain MRANK elements.  When the data is allocated,
	 * space is only allocated for as many dimensions as there
	 * actually are.  Thus, if there are 0 dimensions, no space
	 * will be allocated for "dim".  This had better make the
	 * VAX happy, since it has sure made me unhappy.
	 *
	 * --John Bruner
	 */


	if(rank > MRANK) error("max R");
	i = sizeof *p - SINT * (MRANK-rank);
	if(type == DA) i += size * SDAT;
	else {
		if(type == CH) i += size;
	}
	p = alloc(i);
	p->rank = rank;
	p->type = type;
	p->size = size;
	p->index = 0;
	if(rank == 1) p->dim[0] = size;
	p->datap = (data *)&p->dim[rank];
	return(p);
}

struct item *
dupdat(ap)
struct item *ap;
{
	struct item *p1, *p2;
	int i;

	p1 = ap;
	p2 = newdat(p1->type, p1->rank, p1->size);
	for(i=0; i<p1->rank; i++) p2->dim[i] = p1->dim[i];
	copy(p1->type, p1->datap, p2->datap, p1->size);
	return(p2);
}

copy(type, from, to, size)
char *from, *to;
{
	int i;
	char *a, *b;
	int s;

	if((i = size) == 0) return(0);
	a = from;
	b = to;
	if(type == DA) i *= SDAT;
	if(type == IN) i *= SINT;
	s = i;
	do
		*b++ = *a++;
	while(--i);
	return(s);
}

struct item *
fetch1()
{
	struct item *p;

	p = fetch(sp[-1]);
	sp[-1] = p;
	return(p);
}

struct item *
fetch2()
{
	struct item *p;

	sp[-2] = fetch(sp[-2]);
	p = fetch(sp[-1]);
	sp[-1] = p;
	return(p);
}

struct item *
fetch(ip)
struct item *ip;
{
	struct item *p, *q;
	int i;
	struct nlist *n;
	int c;
	struct chrstrct *cc;
	extern prolgerr;

	p = ip;

loop:
	switch(p->type) {

	case QX:
		aplfree(p);
		n = nlook("Llx");
		if(n){
			q = n->itemp;
			p = dupdat(q);
			copy(q->type, q->datap, p->datap, q->size);
		}
		else p = newdat(CH, 1, 0);
		goto loop;

	case QQ:
		aplfree(p);
		cc = rline(0);
		if(cc == 0) error("eof");
		for(i=0; cc->c[i] != '\n'; i++) ;
		p = newdat(CH, 1, i);
		copy(CH, cc, p->datap, i);
		goto loop;

	case QD:
	case QC:
		printf("L:\n\t");
		i = rline(8);
		if(i == 0) error("eof");
		c = compile(i, 1);
		aplfree(i);
		if(c == 0) goto loop;
		i = pcp;
		execute(c);
		pcp = i;
		aplfree(c);
		aplfree(p);
		p = *--sp;
		goto loop;

	case DU:
		if(lastop != PRINT) error("no fn result");

	case DA:
	case CH:
		p->index = 0;
		return(p);

	case LV:

		/* KLUDGE --
		 *
		 * Currently, if something prevents APL from completing
		 * execution of line 0 of a function, it leaves with
		 * the stack in an unknown state and "gsip->oldsp" is
		 * zero.  This is nasty because there is no way to
		 * reset out of it.  The principle cause of error
		 * exits from line 0 is the fetch of an undefined
		 * function argument.  The following code attempts
		 * to fix this by setting an error flag and creating
		 * a dummy variable for the stack if "used before set"
		 * occurs in the function header.  "ex_fun" then will
		 * note that the flag is high and cause an error exit
		 * AFTER all header processing has been completed.
		 */

		if(((struct nlist *)p)->use != DA){
/*			printf("%s: used before set", ((struct nlist *)ip)->namep);  */
			printf("%s: used before set", ((struct nlist *)p)->namep);
			if ((!gsip) || gsip->funlc != 1) error("");
			q = newdat(DA, 0, 1);		/* Dummy */
			q->datap[0] = 0;
			prolgerr = 1;				/* ERROR flag */
			return(q);
		}
		p = ((struct nlist *)p)->itemp;
		i = p->type;
		if(i == LBL) i = DA;			/* treat label as data */
		q = newdat(i, p->rank, p->size);
		copy(IN, p->dim, q->dim, p->rank);
		copy(i, p->datap, q->datap, p->size);
		return(q);

	default:
		error("fetch B");
	}
}

topfix()
{
	struct item *p;
	int i;

	p = fetch1();
	if(p->type != DA || p->size != 1) error("topval C");
	i = fix(p->datap[0]);
	pop();
	return(i);
}

bidx(ip)
struct item *ip;
{
	struct item *p;

	p = ip;
	idx.type = p->type;
	idx.rank = p->rank;
	copy(IN, p->dim, idx.dim, idx.rank);
	size();
}

size()
{
	int i, s;

	s = 1;
	for(i=idx.rank-1; i>=0; i--) {
		idx.del[i] = s;
		s *= idx.dim[i];
	}
	idx.size = s;
	return(s);
}

colapse(k)
{
	int i;

	if(k < 0 || k >= idx.rank) error("collapse X");
	idx.dimk = idx.dim[k];
	idx.delk = idx.del[k];
	for(i=k; i<idx.rank; i++) {
		idx.del[i] = idx.del[i+1];
		idx.dim[i] = idx.dim[i+1];
	}
	if (idx.dimk) idx.size /= idx.dimk;
	idx.rank--;
}

forloop(co, arg)
int (*co)();
{
	int i;

	if (idx.size == 0) return;
	if (idx.rank == 0) {
		(*co)(arg);
		return;
	}
	i = 0;
	while (1) {
		while(i < idx.rank) idx.idx[i++] = 0;
		(*co)(arg);
		while (1) {
			idx.idx[i-1]++;
			if (idx.idx[i-1] < idx.dim[i-1]) break;
			i--;
			if (i <= 0) return;
		}
	}
}

access()
{
	int i, n;

	n = 0;
	for(i=0; i<idx.rank; i++) n += idx.idx[i] * idx.del[i];
	return(n);
}

data
getdat(ip)
struct item *ip;
{
	struct item *p;
	int i;
	data d;

	/* Get the data value stored at index p->index.  If the
	 * index is out of range it will be wrapped around.  If
	 * the data item is null, a zero or blank will be returned.
	 */

	p = ip;
	i = p->index;
	while(i >= p->size) {
		if (p->size == 0) return((p->type == DA) ? zero : (data)' ')	/* let the caller beware */;
		i -= p->size;
	}
	if(p->type == DA) {
		d = p->datap[i];
	}
	else if(p->type == CH) {
		d = ((struct chrstrct *)p->datap)->c[i];
	}
	else error("getdat B");
	i++;
	p->index = i;
	return(d);
}

putdat(ip, d)
data d;
struct item *ip;
{
	struct item *p;
	int i;

	p = ip;
	i = p->index;
	if(i >= p->size) error("putdat B");
	if(p->type == DA) {
		p->datap[i] = d;
	}
	else if(p->type == CH) {
		((struct chrstrct *)p->datap)->c[i] = d;
	}
	else error("putdat B");
	i++;
	p->index = i;
}


struct item *
s2vect(ap)
struct item *ap;
{
	struct item *p, *q;

	p = ap;
	q = newdat(p->type, 1, 1);
	q->datap = p->datap;
	q->dim[0] = 1;
	return(q);
}

struct nlist *
nlook(name)
char *name;
{
	struct nlist *np;

	for(np = nlist; np->namep; np++) {
		if(equal(np->namep, name)) return(np);
	}
	return(0);
}

checksp()
{
	if(sp >= &stack[STKS]) error("stack overflow");
}

char *
concat(s1,s2)
char *s1, *s2;
{
	int i,j;
	char *p,*q;

	i = strlen(s1);
	j = strlen(s2);
	p = q = alloc(i+j);
	p += copy(CH, s1, p, i);
	copy(CH, s2, p, j);
	return(q);
}

char *
catcode(s1,s2)
char *s1, *s2;
{
	int i,j;
	char *p,*q;

	i = csize(s1) - 1;
	j = csize(s2);
	p = q = alloc(i+j);
	p += copy(CH, s1, p, i);
	copy(CH, s2, p, j);
	return(q);
}

/*
 * csize -- return size (in bytes) of a compiled string
 */
csize(s)
char *s;
{
	int c,len;
	char *p;
	int i;

	len = 1;
	p = s;
	while((c = *p++) != EOF){
		len++;
		c &= 0377;
		switch(c){
		default:
			i = 0;
			break;

		case QUOT:
			i = *p++;
			break;

		case CONST:
			i = *p++;
			i *= SDAT;
			len++;
			break;

		case NAME:
		case FUN:
		case ARG1:
		case ARG2:
		case AUTO:
		case REST:
		case RVAL:
			i = SINT;	/*	All pointers better be the same size as int's  */
			break;
		}
		p += i;
		len += i;
	}
	return(len);
}

opn(file, rw)
char file[];
{
	int fd, (*p)();
	char f2[100];
	extern open(), creat();

	p = (rw > 2 ? creat : open);
	if((fd = (*p)(file,rw)) < 0){
		for(fd=0; fd<13; fd++) f2[fd] = "/usr/lib/apl/"[fd];
		for(fd=0; file[fd]; fd++) f2[fd+13] = file[fd];
		f2[fd+13] = 0;
		if((fd = (*p)(f2, rw)) >= 0){
			printf("[using %s]\n", f2);
			return(fd);
		}
		printf("can't open file %s\n", file);
		/* error(""); */
	}
	return(fd);
}

catchsigs()
{
	extern panic();

return; /* fixme */

#ifdef SIGHUP
	signal(SIGHUP, panic);
#endif
#ifdef SIGQUIT
	signal(SIGQUIT, panic);
#endif
#ifdef SIGILL
	signal(SIGILL, panic);
#endif
#ifdef SIGTRAP
	signal(SIGTRAP, panic);
#endif
#ifdef SIGEMT
	signal(SIGEMT, panic);
#endif
#ifdef SIGBUS
	signal(SIGBUS, panic);
#endif
	signal(SIGSEGV, panic);
#ifdef SIGSYS
	signal(SIGSYS, panic);
#endif
#ifdef SIGPIPE
	signal(SIGPIPE, panic);
#endif
#ifdef SIGTERM
	signal(SIGTERM, panic);
#endif
#ifdef SIGABRT
	signal(SIGABRT, panic);
#endif
}

panic(signum)
unsigned signum;
{

	int fd;
	static insane = 0;			/* if != 0, die */
	static char *abt_file = "aplws.abort";
	static char *errtbl[] = {
		"excessive eofs",
		"hangup",
		"interrupt",
		"quit",
		"illegal instruction",
		"trace trap",
		"i/o trap instruction",
		"emt trap",
		"floating exception",
		"kill",
		"bus error",
		"segmentation violation",
		"bad system call",
		"write no pipe",
		"alarm clock",
		"software termination"
	};

	if (normalExit) exit(0);

	/* Attempt to save workspace.  A signal out of here always
	 * causes immediate death.
	 */

	mencflg = 0;
	signal(signum, panic);
	/* printf("\nfatal signal: %s\n", errtbl[(signum < NSIG) ? signum : 0]); */
	printf("\nfatal signal: %d\n", signum);

	if (mkcore) abort();

	if (!insane++){
		if ((fd=creat(abt_file, 0644)) >= 0){
			printf("[attempting ws dump]\n");
			wssave(fd, 0);
			printf(" workspace saved in %s\n", abt_file);
			close(fd);
		}
		else printf("workspace lost -- sorry\n");
	}
	else printf("recursive errors: unrecoverable\n");

	term(0);
}


empty(fd){

	struct stat sbuf;
	struct fds *fp;

	/* Simulate the Rand Corp.'s "empty" system call on a
	 * V7 system by seeing if the given fd is a pipe, and if
	 * so, whether or not it is empty.
	 */

	if (fstat(fd, &sbuf) < 0) return(-1);		/* Can't "stat" it */
	return(sbuf.st_size == 0);
}
