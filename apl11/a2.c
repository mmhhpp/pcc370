#include "apl.h"
/* #include <varargs.h> */

extern int chartab[];
char *ecvt();

ex_print()
{
	if(epr0()) putchar('\n');
}

ex_hprint()
{
	epr0();
	pop();
}

epr0()
{
	struct item *p;
	data *dp;
	int i, j, param[4];

	p = fetch1();
	if(p->type == DU) return(0);
	if(p->size == 0) return(1);
	if(p->type == DA) {

		/* Use "epr1()" to figure out the maximum field width
		 * required by any of the values to be printed.
		 */

		for(i=0; i<4; i++) param[i] = 0;
		dp = p->datap;
		for(i=0; i<p->size; i++) epr1(*dp++, param);
		i = param[1] + param[2]; /* size if fp */
		if(i > thread.digits) i += 100;               /* set "e" format flag */
		if(param[2]) i++;
		if(i > param[0]+5) {
			i = param[0] + 5; /* size if ep */
			param[1] = param[0];
			param[2] = -1;
		}
		if(param[3]) i++;	/* sign */
		i++;		/* leading space */
		param[0] = i;
		dp = p->datap;
	}
	bidx(p);
	for(i=1; i<p->size; i++) {
		if(intflg) break;
		if(p->type == CH) {
			j = getdat(p);
			putchar(j);
		}
		else epr2(*dp++, param);
		for(j=p->rank-2; j>=0; j--) {
			if(i%idx.del[j] == 0) putchar('\n');  /* end of dimension reached */
		}
	}
	if(p->type == CH) {
		j = getdat(p);
		putchar(j);
	}
	else epr2(*dp, param);
	return(1);
}

epr1(d, param)
data d;
int *param;
{
	double f;
	int a;
	char *c;
	int dp, sg;

	/* This routine figures out the field with required by the value
	 * "d".  It adjusts the four elements of "param" so that they
	 * contain the maximum of their old values or the requirements for
	 * the current data item.
	 *
	 * param[0] = number of significant digits
	 * param[1] = number of digits to left of decimal point
	 * param[2] = number of digits to right of decimal point
	 * param[3] = 0 if positive, 1 if negative
	 */

	f = d;
	c = ecvt(f, thread.digits, &dp, &sg);
	if (f == zero) dp = 1;             		/* kludge due to change in ecvt */
	a = thread.digits;
	while(c[a-1]=='0' && a>1) a--;
	if(a > param[0]) param[0] = a;			/* sig digits */
	a -= dp;
	if(a < 0) a = 0;
	if(a > param[2]) param[2] = a;			/* digits to right of dp */
	if(dp > param[1]) param[1] = dp;		/* digits to left of dp */
	param[3] |= sg;							/* and sign */
}

epr2(d, param)
int *param;
data d;
{
	int i, dp, sg;
	char *c, *mc;
	double f;

	if(param[0]+column > thread.width && !mencflg) {
		putchar('\n');
		putto(param[0]);
	}
	f = d;
	c = ecvt(f, thread.digits, &dp, &sg);
	if (f == zero) dp = 1;					/* kludge due to change in ecvt */
	mc = c + thread.digits;
	putchar(' ');
	sg = sg? '-': ' ';						/* '-' used to be '"' */
	if(param[2] < 0) {
		if(param[3]) putchar(sg);
		for(i=0; i<param[1]; i++) {
			putchar(*c++);
			if(i == 0) putchar('.');
		}
		putchar('e');
		dp--;
		if(dp < 0) {
			putchar('-');					/* '=' used to be '"' */
			dp = -dp;
		}
		else putchar('+');					/* apl style plus sign, used to be ':' */
		putchar(dp/10 + '0');
		putchar(dp%10 + '0');
		return;
	}
	i = dp;
	if(i < 0) i = 0;
	for(; i<param[1]; i++) putchar(' ');
	if(param[3]) putchar(sg);
	for(i=0; i<dp; i++) {
		if(c >= mc) putchar('0');
		else putchar(*c++);
	}
	for(i=0; i<param[2]; i++) {
		if(i == 0) putchar('.');
		if(dp < 0) {
			putchar('0');
			dp++;
		}
		else {
			if(c >= mc) putchar('0');
			else putchar(*c++);
		}
	}
}

error(s)
char *s;
{
	int c;
	char *cp, *cs;

	intflg = 0;
	if(ifile) {
		close(ifile);
		ifile = 0;
	}
	cp = s;
	while(c = *cp++) {
		if(c >= 'A' && c <= 'Z') {
			switch(c) {

			case 'I':
				cs = "\ninterrupt";
				break;

			case 'L':
				cs = "L";
				break;

			case 'C':
				cs = "conformability";
				break;

			case 'S':
				cs = "syntax";
				break;

			case 'R':
				cs = "rank";
				break;

			case 'X':
				cs = "index";
				break;

			case 'Y':
				cs = "character";
				break;

			case 'M':
				cs = "memory";
				break;

			case 'D':
				cs = "domain";
				break;

			case 'T':
				cs = "type";
				break;

			case 'E':
				cs = "error";
				break;

			case 'P':
				cs = "programmer";
				break;

			case 'B':
				cs = "botch";
				break;

			default:
				putchar(c);
				continue;
			}
			printf(cs);
			continue;
		}
		putchar(c);
	}
	putchar('\n');
	if (prwsflg) exit(0);				/* if "prws", just exit */
	/*
	 * produce traceback and mark state indicator.
	 */
	tback(0);
	if(gsip) gsip->suspended = 1;
	else {
		while(sp > stack) pop();		/* zap garbage */
		reset();
	}
	mainloop();
}
#if 0
printf(va_alist)
va_dcl
{
	va_list pvar;
	char *s, *cp;
	int p;
	data d;

	va_start(pvar);
	s = va_arg(pvar, char *);

	while(*s) {
		if(s[0] == '%') {
			switch(s[1]){

				case 'd':
				p = va_arg(pvar, int);
				putn(p);
				s += 2;
				continue;

				case 'o':
				p = va_arg(pvar, int);
				puto(p);
				s += 2;
				continue;

				case 's':
				cp = va_arg(pvar, char *);
				s += 2;
				while(*cp) putchar(*cp++);
				continue;

				case 'f':
				d = va_arg(pvar, double);
				putf(&d);
				s += 2;
				continue;
			}
		}
		putchar(*s);
		s++;
	}
}
#endif
putn(n)
{
	int a;

	if (n < 0) {
		n = -n;
		if (n < 0) {
			printf("32768");
			return;
		}
		putchar('-');				/* apl minus sign, was '"' */
	}
	if (a = n / 10) putn(a);
	putchar(n % 10 + '0');
}

putf(p)
data *p;
{
	int i, param[4];

	param[1] = param[2] = param[3] = param[0] = 0;
	epr1(*p, param);
	i = param[1] + param[2];		/* size if fp */
	if(i > thread.digits) i += 100;
	if(param[2]) i++;
	if(i > param[0]+5) {
		i = param[0] + 5;			/* size if ep */
		param[1] = param[0];
		param[2] = -1;
	}
	if(param[3]) i++;				/* sign */
	i++;							/* leading space */
	param[0] = i;
	epr2(*p, param);
}

puto(n)
{
	if(n&0177770) puto( (n>>3) & 017777);
	putchar( '0' + (n&07));
}
#if 0
getchar()
{
	char c;
	int count;

	c = 0;
	count = read(ifile, &c, 1);
	if (count == 1 && echoflg == 1 && ifile == 0) putchar(c);
	if (c && protofile && ifile == 0) write(protofile, &c, 1);
	return(c);
}

putchar(d)
{
	char c;
	int i;

	c = d;
	if(mencflg) {
		if(c != '\n') {
			mencflg = 1;
			*mencptr++ = c;
		}
		else {
			if(mencflg > 1) mencptr += rowsz;
			else mencflg = 2;
		}
		return;
	}

	switch(c){

		case '\0':
		return;

		case '\b':
		if(column) column--;
		break;

		case '\t':
		column = (column+8) & ~7;
		break;

		case '\r':
		case '\n':
		column = 0;
		break;

		default:
		column++;
	}

	if (column > thread.width) printf("\n    ");

	if(intflg == 0) {
		if(c & 0200) {
			i = chartab[c & 0177];
			putchar(i>>8);
			c = i & 0177;
			putchar('\b');
		}
		if(protofile) write(protofile, &c, 1);
		write(1, &c, 1);
	}
}
#endif

char *ty[] = {
0,"DA","CH","LV","QD","QQ","IN","EL","NF","MF","DF","QC","QV","DU","QX","LB",
};

dstack()
{
	struct item **p;
	int i,n;

	p = sp;
	n = 0;
	while(--p > stack){
		printf("\t%o:  sp[%d]:   type = ", p, --n);
		if((i=(*p)->type) >= 0 && i <= LBL && ty[i]) printf(ty[i]);
		else printf("%d", (*p)->type);
		switch(i){
		default:
			putchar('\n');
			break;
		case LV:
			printf(",  n = %s\n", ((struct nlist *)*p)->namep);
			break;

		case CH:
			if((*p)->size == 0) goto nullone;
			if((*p)->rank == 1){
				printf(",  \"");
				for(i=0; i<(*p)->size; i++) putchar(((struct chrstrct *)(*p)->datap)->c[i]);
				printf("\"\n");
			}
			else goto rnk;
			break;

		case DA:
		case LBL:
			if((*p)->size == 0) goto nullone;
			if((*p)->rank == 0) printf(",  v = %f\n", (*p)->datap[0]);
			break;
		rnk:
			printf(",  rank = %d\n", (*p)->rank);
			break;

		nullone:
			printf(",  <null>\n");
			break;
		}
	}
	putchar('\n');
}
