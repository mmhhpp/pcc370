#include "apl.h"

ex_scn0()
{
	fetch1();
	scan0(0);
}

ex_scan()
{
	struct item *p;

	p = fetch1();
	scan0(p->rank-1);
}

ex_scnk()
{
	int i;

	i = topfix() - thread.iorg;
	scan0(i);
}

scan0(k)
{
	struct item *p, *q;
	data *param[2];
	int scan1();

	p = fetch1();
	if(p->type != DA) error("scan T");
	bidx(p);
	colapse(k);
	if(idx.dimk == 0) {
/*
 *  scan identities - ets/jrl 5/76
 */
		q = newdat(DA,0,1);
		q->dim[0] = 1;
		switch(*pcp++) {
	case ADD:
	case SUB:
	case OR:
			q->datap[0] = 0;
			break;
	case AND:
	case MUL:
	case DIV:
			q->datap[0] = 1;
			break;
	case MIN:
			q->datap[0] = 1.0e38;
			break;
	case MAX:
			q->datap[0] = -1.0e38;
			break;
	default:
			error("reduce identity");
		}
		pop();
		*sp++ = q;
		return;
	}
	param[0] = p->datap;
	param[1] = (data *)exop[*pcp++];
	forloop(scan1, param);
}

scan1(param)
data *param[];
{
	int i, j;
	data *dp, *ip, d, (*f)();

	f = (data (*)())param[1];
	for (i=0; i<idx.dimk; i++) {
		dp = param[0] + access() + (idx.dimk - (i+1)) * idx.delk;
		ip = dp;
		d  = *ip;
		for (j=1; j<idx.dimk-i; j++) {
			ip -= idx.delk;
			d = (*f)(*ip, d);
		}
		*dp = d;
	}
}

/*
 *	Everything from here to the eof is commented out.  This appears
 *	to be some kind of graphics stuff, but very much specific to
 *	things as they were at Purdue.  Come back and actually remove
 *	this once I have all the related stuff pinned down -- MEC
 *
data scalex = 453.;
data scaley = 453.;
data origx = 0.0;
data origy = 0.0;

ex_plot()
{
	struct item *p;
	data *dp;
	int i, ic, x, y;

	p = fetch1();
	if(p->type != DA) error("plot T");
	if(p->rank != 2) error("plot R");
	if(p->dim[1] != 2) error("plot C");

	dp = p->datap;
	if ((i = p->dim[0]) == 0) return;
	ic=0;
	while(i--) {
		x = scalex*(*dp++ - origx);
		y = 454-(scaley*(*dp++ - origy));
		if(x<0 || x >= 576 || y<0 || y>=454) error("plot off screen");
		if(ic) line(x,y);
		else {
			move(x,y);
			ic=1;
		}
	}
}

line(x,y)
{
}

move(x,y)
{
}
 */
