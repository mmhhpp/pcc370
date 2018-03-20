#include "apl.h"

ex_rav()
{
	struct item *p, *r;

	p = fetch1();
	if(p->rank == 0) {
		r = newdat(p->type, 1, 1);
		putdat(r, getdat(p));
		pop();
		*sp++ = r;
		return;
	}
	rav0(p->rank-1);
}

ex_ravk()
{
	int i;

	i = topfix() - thread.iorg;
	fetch1();
	rav0(i);
}

rav0(k)
{
	struct item *p, *r, *param[2];
	int rav1();

	p = sp[-1];
	bidx(p);
	colapse(k);
	r = newdat(p->type, 1, p->size);
	param[0] = p;
	param[1] = r;
	forloop(rav1, param);
	pop();
	*sp++ = r;
}

rav1(param)
struct item *param[];
{
	struct item *p;
	int i, n;

	p = param[0];
	n = access();
	for(i=0; i<idx.dimk; i++) {
		p->index = n;
		putdat(param[1], getdat(p));
		n += idx.delk;
	}
}

ex_cat()
{
	struct item *p, *q, *r;
	int k;

	p = fetch2();
	q = sp[-2];
	k = p->rank;
	if(q->rank > k) k = q->rank;
	if(k == 0) {
		r = newdat(p->type, 1, 2);
		putdat(r, getdat(p));
		putdat(r, getdat(q));
		pop();
		pop();
		*sp++ = r;
	}
	else cat0(k-1);
}

ex_catk()
{
	int k;
	double d, top();

	d = top();
	k = fix(d);
	fetch2();
	if (0 == fuzz(d, (double)k)) cat0(k-1);
	else lam0(d);
}

cat0(k)
{
	struct item *p, *q, *r;
	int i, a, b;

	p = sp[-1];
	q = sp[-2];
	i = k;
	if(p->rank >=  q->rank) {
		bidx(p);
		b = cat1(q, i);
		a = idx.dim[i];
	}
	else {
		bidx(q);
		a = cat1(p, i);
		b = idx.dim[i];
	}
	idx.dim[i] = a+b;
	size();
	r = newdat(p->type, idx.rank, idx.size);
	copy(IN, idx.dim, r->dim, idx.rank);
	i = idx.del[i];
	a *= i;
	b *= i;
	while(r->index < r->size) {
		for(i=0; i<a; i++) putdat(r, getdat(p));
		for(i=0; i<b; i++) putdat(r, getdat(q));
	}
	pop();
	pop();
	*sp++ = r;
}

cat1(ip, k)
struct item *ip;
{
	struct item *p;
	int i, j, a;

	if(k < 0 || k >= idx.rank) error("cat X");
	p = ip;
	a = 1;
	if(p->rank == 0) return(a);
	j = 0;
	for(i=0; i<idx.rank; i++) {
		if(i == k) {
			if(p->rank == idx.rank) {
				a = p->dim[i];
				j++;
			}
			continue;
		}
		if(idx.dim[i] != p->dim[j]) error("cat C");
		j++;
	}
	return(a);
}


double
top()
{
	struct item *p;
	double d;

	p = fetch1();
	if (p->type != DA || p->size != 1) error("topval C");
	d = p->datap[0];
	pop();
	return d;
}


lam0(d)
double d;
{
	struct item *p, *q, *r;
	int i, j, k;

	p = sp[-1];
	q = sp[-2];
	if (q->rank > p->rank) p = q;
	if (p->rank >= MRANK) error("lam X");
	idx.type = p->type;
	idx.rank = p->rank + 1;
	k = fix(d) - thread.iorg;
	if (k < 0 || k >= p->rank+1) error("lam X");
	j = 0;
	for (i=0; i<p->rank; i++) {
		if (i == k) idx.dim[j++] = 1;
		idx.dim[j++] = p->dim[i];
	}
	if (i == k) idx.dim[j] = 1;
	size();
	r = newdat(idx.type, idx.rank, idx.size);
	copy(IN, idx.dim, r->dim, idx.rank);
	copy(idx.type, p->datap, r->datap, r->size);
	if (p == sp[-1]) sp[-1] = r;
	else sp[-2] = r;
	aplfree(p);
	cat0(k);
}
