/*
 *	monadic epsilon and encode /rww
 */
#include "apl.h"
#include <signal.h>

ex_meps()
{
    struct item *p;
    int i, j, dim0, dim1;
    char *a,*b,*c, *xpcp;

	p = fetch1();
	if ( p->rank > 2 || p->type != CH ) error("execute C");
	/*get out if nothing to do, apr 2-23-77 */
	if (p->size == 0) return;
	b = (char *)p->datap;
	dim0 = p->rank < 2 ? 1       : p->dim[0];
	dim1 = p->rank < 2 ? p->size : p->dim[1];
	a = alloc ( dim1+1 );
	xpcp = pcp;
	for ( i=0; i<dim0 ; i++) {
		copy(CH, b, a, dim1);
		a[dim1] = '\n';
		c = compile(a,1);
		if(c != 0){
			execute(c);
			aplfree(c);
		}
		else {
			aplfree(a);
			error("");
		}
		b += dim1;
		if(i < dim0-1) pop();
	}
	aplfree(a);
	pcp = xpcp;
	p = *--sp;
	pop();
	*sp++ = p;
}

ex_menc()
{
	struct item *p;

	p = fetch1();
	if ( p->type == DA ) menc1();
}


ex_list()	/* List a function on the terminal */
{
	char lastc, c;
	struct nlist *n;
	int line;

	/* Check for valid function */

	n = (struct nlist *)*--sp;
	if (n->type != LV) error("fnlist B");

	/* If a function, locate it in workspace file and
	 * print on the terminal in formatted form.
	 */

	switch(((struct nlist *)n)->use){
	default:
		error("fnlist T");

	case NF:
	case MF:
	case DF:
		lseek(wfile, (long)n->label, 0);
		line = 0;
		lastc = 0;
		putchar('\n');

		while(read(wfile, &c, 1) > 0){

			if (!c){
				putchar('\n');
				return;
			}

			switch(lastc){
			case '\n':
				printf("[%d]", ++line);
			case 0:
				putchar('\t');
			}
			putchar(lastc=c);
		}
		error("workspace eof");
	}
}


ex_crp()		/* dredge up a function and put it into an array*/
{
	char name[NAMS], *c, *c2, *dp;
	struct nlist *np;
	struct item *p;
	int len, dim0, dim1, i;

	p = fetch1();
	if ( p->size == 0 || p->rank >1 || p->size >= NAMS ) error("Lcr C");

				/* set up the name in search format     */

	copy(CH, p->datap, name, p->size);
	name[p->size] = '\0';
	np = nlook(name);

				/* if not found then domain error       */

	if ( !np->namep ) error("Lcr D");
	switch(np->use){
	default:
		error("Lcr D");
	case MF:
	case DF:
	case NF:	/* only allow functions */
		;
	}
				/* set up new array                     */
	dim0 = 0;
	dim1 = 0;
	ifile = dup(wfile);

				/* look up function     */

	lseek( ifile, (long)np->label, 0);

				/* compute max width and height         */

	while ( c2 = c = rline(0) ){
	    while ( *c2++ != '\n' ) ;
		dim0++;
		len = c2 - c - 1;
		dim1 = dim1 < len ? len : dim1;
		aplfree(c);
	}
	pop();		/* release old variable         */

				/* create new array and put function in */

	p = newdat ( CH, 2, dim0*dim1 );
	p->rank = 2;
	p->dim[0] = dim0;
	p->dim[1] = dim1;
	dp = (char *)(p->datap);
	lseek( ifile, (long)np->label, 0);
	while ( c2 = c = rline(0) ){
	    for ( i=0; i<dim1; i++) {
			if ( *c != '\n' ) *dp++ = *c++;
			else *dp++ = ' ';    /* fill w/blanks*/
		}
		aplfree(c2);
	}

				/* put the new array on the stack       */
	*sp++ = p;

				/* reset the current file               */
	close(ifile);
	ifile = 0;
}

menc1()			/* change numbers into characters       */
{
	struct item *p, *q;
	int i,j,numsz, total,param[4];
	data *dp;

				/* zeroize size information vector      */
	for ( i=0; i<4; i++ ) param[i] = 0;

				/* pick up the argument                 */
	p = fetch1();
	if(p->rank > 2) error("format R");
	dp = p->datap;

				/* find the maximum # of chars in any # */
	for(i=0; i<p->size; i++) epr1(*dp++, param);
	numsz = param[1] + param[2] + !!param[2] + param[3] + 1;

				/* rowsize is max # size x last dim     */
	rowsz = p->rank ? p->dim[p->rank-1] : 1;
	rowsz *= numsz;

				/* row size x # of rows (incl blank)    */
	total = p->size * numsz;
	for( j=i=0; i<p->rank; i++ ) {
		if ( p->dim[i] != 1) {
			 if (j++ > 1 ) total += rowsz;
		}
	}

				/* make new data and fill with blanks   */
	if(p->rank == 2){
		q = newdat(CH, 2, total);
		q->dim[0] = total/rowsz;
		q->dim[1] = rowsz;
	}
	else {
		q = newdat( CH, 1, total);
		q->dim[0] = rowsz;
	}
	mencptr = (char *)(q->datap);
	for ( i=0; i<total; i++) *mencptr++ = ' ';
	mencptr = (char *)(q->datap);

				/* use putchar() to fill up the array   */
	mencflg = 2;
	ex_hprint();
	mencflg = 0;

				/* put it on the stack                  */
	*sp++ = q;
}


ex_run()
{
	struct item *p;
	data *dp;
	char ebuf[100];
	int i, val;

	p = fetch1();
	if(p->type != CH || p->rank != 1) error("Lrun D");
	copy(CH, p->datap, ebuf, p->size);
	ebuf[p->size] = 0;
	val = system(ebuf);
	p = newdat(DA, 0, 1);
	p->datap[0] = (data)val;
	pop();
	*sp++ = p;
}


ex_dfmt()
{
	char *cp, *ecp;
	data *fp, *dp;
	int j, i, sign, decpt;
	struct item *lp, *rp, *ip;
	unsigned nrow, ncol, rowlen, inc, wid;

	/* Dyadic format.  This routine is a little crude and should
	 * probably be rewritten to take advantage of other conversion
	 * routines.  Nonetheless, it does do dyadic formatting for
	 * scalars, vectors, and 2-dimensional arrays when the left
	 * argument is a 2-element or appropriate-length vector
	 * specifying non-exponential ("F format") conversion.
	 */

	lp = fetch2();
	rp = sp[-2];
	nrow = (rp->rank < 2) ? 1 : rp->dim[0];
	ncol = rp->rank ? rp->dim[rp->rank-1] : 1;
	inc = (lp->size != 2) * 2;

	/* Check validity of arguments. */

	if (lp->rank > 1 || lp->size <= 1 || rp->rank > 2
	    || lp->type != DA || rp->type != DA
	    || (lp->size != 2 && lp->size != 2*ncol))
		error("dfmt D");

	for(fp=lp->datap,i=0; i < lp->size; i += 2,fp += 2){
		if (fp[0] <= 0.0 || fp[1] < 0.0) error("dfmt D");
		fp[0] = (data)((int)(0.5+fp[0]));
		fp[1] = (data)((int)(0.5+fp[1]));
	}

	/* Allocate result array */

	for(i=rowlen=0,fp=lp->datap; i < ncol; i++, fp += inc) rowlen += (int)*fp;

	ip = newdat(CH, rp->rank ? rp->rank : 1, rowlen*nrow);

	if (rp->rank < 2) ip->dim[0] = rowlen;
	else {
		ip->dim[0] = nrow;
		ip->dim[1] = rowlen;
	}

	cp = (char *)ip->datap;
	dp = rp->datap;
	while(nrow--) {
		for(i=0,fp=lp->datap; i < ncol; i++, dp++, fp += inc){
			int avail, whole, frac, count;
			char buffer[256], *ecvt();

repeat:
			avail = wid = (int)(fp[0] + 0.5);
			frac = (int)(fp[1] + 0.5);
			if (frac > 0) avail--;			/*	Space for the decimal point.  */
			if (*dp < 0) avail--;			/*	Space for the minus sign.  */
			whole = avail - frac;

								/*	Handle zero as a special case.  */

			if (*dp == 0.0) {
				for (j=0; j<whole-1; j++) *cp++ = ' ';
				if (frac) {
					*cp++ = ' ';
					*cp++ = '.';
					for (j=0; j<frac; j++) *cp++ = '0';
				}
				else *cp++ = '0';
				continue;
			}
								/*	Format the number, and deal with a whole
									raft of special cases: overflow, underflow,
									and rounding numbers to integers when we're
									not printing a fractional part.  */

			ecp = ecvt(*dp, avail, &decpt, &sign);
			if (decpt > whole) {
				for (j=0; j<wid; j++) *cp++ = '*';
				continue;
			}
			if (frac < -decpt) {
				*dp = 0;
				goto repeat;
			}
			if (frac == 0 && decpt == 0) {
				if (sign) *dp -= 0.5;
				else *dp += 0.5;
				*dp = (int)*dp;
				goto repeat;
			}
								/*	Convert the value again, using the proper number
									of digits (ie, let ecvt() do the rounding, not me).
									Then copy the digits, with leading blanks, minus
									sign and decimal point as needed.  */

			if (frac+decpt != avail) ecp = ecvt(*dp, frac+decpt, &decpt, &sign);
			count = (decpt >= 0) ? whole-decpt : whole;
			for (j=0; j<count; j++) *cp++ = ' ';
			if (sign) *cp++ = '-';
			if (decpt >= 0) for (j=0; j<decpt; j++) *cp++ = *ecp++;
			if (frac) {
				*cp++ = '.';
				if (decpt < 0) {
					for (j=0; j<-decpt; j++) *cp++ = '0';
					count = frac+decpt;
				}
				else count = frac;
				for (j=0; j<count; j++) *cp++ = *ecp++;
			}
		}
	}

	pop();
	pop();
	*sp++ = ip;
}

ex_mfmt()
{
	ex_menc();
}

ex_nc()
{
	struct nlist *np;
	struct item *p;
	int i;
	char buf[40], *q;

	p = fetch1();
	if(p->type != CH) error("Lnc T");
	if(p->size >= 40 || p->rank > 1) error("Lnc D");
	copy(CH, p->datap, buf, p->size);
	buf[p->size] = 0;
	np = nlook(buf);
	i = 0;
	if(np != 0) {
		switch(np->use){
		case 0:
			i = 0; break;
		case MF:
		case NF:
		case DF:
			i = 3; break;
		case DA:
		case CH:
		case LV:
			i = 2; break;
		default:
			printf("unknown Lnc type = %d\n", np->use);
			i = 4;
		}
	}
	p = newdat(DA, 0, 1);
	p->datap[0] = i;
	pop();
	*sp++ = p;
}

ex_nl()
{

	struct item *ip;
	struct nlist *np;
	data *dp;
	char *cp, *cp2, tlist[NTYPES];
	int i, count, maxlen;

	/* Namelist quad function.  This is monadic (dyadic not
	 * implemented).  The argument is a list of types:
	 *  1:	labels
	 *  2:	variables
	 *  3:	functions
	 * whose names are desired.  The result is a character array
	 * containing all defined names (in no particular order) of
	 * the specified type(s).  The number of rows in the matrix
	 * is the number of names; the number of columns is the
	 * same as the longest name (other names are space-filled).
	 */

	ip = fetch1();
	if (ip->rank > 1 || ip->type != DA) error("Lnl D");

	for(i=0; i < NTYPES; i++) tlist[i] = 0;
	for(dp=ip->datap; dp < ip->datap+ip->size; dp++) {
		switch((int)*dp){
		case 1:	tlist[LBL] = 1; break;
		case 2:	tlist[CH] = tlist[DA] = 1; break;
		case 3:	tlist[NF] = tlist[MF] = tlist[DF] = 1; break;
		default:error("Lnl D"); break;
		}
	}

	count = maxlen = 0;
	for(np=nlist; np < &nlist[NLS]; np++){
		if (np->use < NTYPES && tlist[np->use]){
			count++;
			if ((i=strlen(np->namep)) > maxlen) maxlen = i;
		}
	}


	ip = newdat(CH, 2, count*maxlen);
	ip->dim[0] = count;
	ip->dim[1] = maxlen;
	cp = ip->datap;

	for(np=nlist; np < &nlist[NLS]; np++) {
		if (np->use < NTYPES && tlist[np->use]) {
			for(cp2 = &np->namep[i=0]; i < maxlen; i++) {
				if (*cp2) *cp++ = *cp2++;
				else *cp++ = ' ';
			}
		}
	}

	pop();
	*sp++ = ip;
}

ex_prws(){

	struct nlist *np;
	struct item *ip;
	int i;

	/* Print workspace in ASCII format */

	printf("origin = %d\nwidth = %d\ndigits = %d\n\n\n", thread.iorg, thread.width, thread.digits);
	for(np=nlist; np < &nlist[NLS]; np++) {
		switch(np->use){
		case CH:
		case DA:
			printf("%s { ", np->namep);
			ip = np->itemp;
			if (ip->rank) {
				for(i=0; i < ip->rank; i++) printf("%d ", ip->dim[i]);
				printf("R\n");
			}
			*sp++ = np;
			ex_print();
			pop();
			putchar('\n');
			break;

		case NF:
		case MF:
		case DF:
			*sp++ = np;
			ex_list();
			putchar('\n');
			break;
		}
	}
}
