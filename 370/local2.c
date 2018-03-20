#include <string.h>
# include "mfile2"

extern FILE *outfile;
extern negrel[];

extern int dimtab[];

static int ilfield;
static int halfword;

/* a lot of the machine dependent parts of the second pass */

# define BITMASK(n) ((1L<<n)-1)

lineid( l, fn ) char *fn; {
	/* identify line l and file fn */
	printf( "/	line %d, file %s\n", l, fn );
	}

extern isstatc();
extern markref();
eobl2(){
	OFFSZ spoff;	/* offset from stack pointer */

	spoff = maxoff;
	if( spoff >= AUTOINIT ) spoff -= AUTOINIT;
	spoff /= SZCHAR;
	SETOFF(spoff,2);
	if( fltused ) {
		fltused = 0;
		}
	}

struct hoptab { int opmask; char * opstring; } ioptab[]= {

	ASG PLUS, "add",
	ASG MINUS, "sub",
	ASG OR,	"bis",
	ASG AND,	"bic",
	ASG ER,	"xor",
	ASG MUL, "mul",
	ASG DIV, "div",
	ASG MOD, "div",
	ASG LS,	"asl",
	ASG RS,	"asr",

	-1, ""    };

hopcode( f, o ){
	/* output the appropriate string from the above table */

	register struct hoptab *q;

	for( q = ioptab;  q->opmask>=0; ++q ){
		if( q->opmask == o ){
			printf( "%s", q->opstring );
			if( f == 'F' ) printf( "f" );
			return;
			}
		}
	cerror( "no hoptab for %s", opst[o] );
	}

char *
rnames[]= {  /* keyed to register number tokens */

	"0", "1", "2", "3",
	"4", "5", "6", "7",
	"8", "9", "10", "11",
	"12", "13", "14", "15",
#if 1
	"@FR0", "@FR2", "@FR4", "@FR6"       /* fp regs */
#else
	"0", "2", "4", "6"               /* fp regs */
#endif
	};

int rstatus[] = {
	SAREG       , SAREG       , SAREG|STAREG, SAREG|STAREG,
	SAREG|STAREG, SAREG|STAREG, SAREG|STAREG, SAREG|STAREG,
	SAREG|STAREG, SAREG|STAREG, SAREG       , SAREG       ,
	SAREG       , SAREG       , SAREG       , SAREG|STAREG,
	SBREG|STBREG, SBREG|STBREG, SBREG|STBREG, SBREG|STBREG
	};

NODE *brnode;
int brcase;

int arglvl = 0;
int arglmax = 0;
#define ARGLVLS 16
int arglvlo[ARGLVLS];	/* starting offset of args at this level, in words */
int arglvlt[ARGLVLS];	/* number of args (words) */
int arglvlc[ARGLVLS];	/* next arg word to be assigned */

zzzcode( p, c ) NODE *p; {
	register m;
	int r;
	int oreg_r, oreg_val, treg_r, freg_r;
	switch( c ){

	case 'G':
		printf ("%ld", p->right->lval & 255);
		return;

	case 'J':
		r = getlr( p, '1')->rval;
		printf ("l    %d", r);
		return;

	case 'K':
		fprintf ( outfile, "          DC    ");
		conput ( getlr( p, 'L') );
		fprintf ( outfile, "     zk line %d\n", lineno );
		return;

	case 'B':
		if ( p->op != ICON )
			cerror ("ZB not ICON");
		fprintf ( outfile, "%ld", p->lval  & 0xff );
		return;

	case 'N':  /* logical ops, turned into 0-1 */
		/* use register given by register 1 */
		cbgen( 0, m=getlab(), 'I' );
		deflab( p->label, 1 );
		printf( "          slr   %s,%s   zn1\n", rnames[getlr( p, '1' )->rval], rnames[getlr( p, '1' )->rval] );
		deflab( m, 1 );
		return;

	case 'I':
	case 'F':
		cbgen( p->op, p->label, c );
		return;

	case 'A':
	case 'C':
		/* logical operators for longs
		   defer comparisons until branch occurs */

		brnode = tcopy( p );
		brcase = c;
		return;

	case 'H':
		halfword = 1;
		return;

	case 'V':
		/* sign extend or not -- register is one less than the
		   left descendent */

		m = p->left->rval - 1;

		if( ISUNSIGNED(p->type) ){
			printf( "	clr	r%d\n", m );
			}
		else {
			printf( "	sxt	r%d\n", m );
			}
		return;

		/* stack management macros */
	case '-':
		arglvlc[arglvl-1] --;
		if (arglvlc[arglvl-1] < arglvlo[arglvl-1])
			cerror ("arglvl: overrun");
#if 0
		if (ilfield) {
			printf( "$B%05d+%d(%d,%d)" , ftnno, arglvlc[arglvl-1]*4, ilfield, STKREG );
			ilfield = 0;
			}
		else
			printf( "$B%05d+%d(%d)" , ftnno, arglvlc[arglvl-1]*4, STKREG );
#endif
		printf("          L    1,=AL4($B%05d+%d)   arg %d arg1\n", ftnno, arglvlc[arglvl-1]*4, arglvlc[arglvl-1] );
		printf("          AR   1,13    arg2\n");
		return;

	case '8':
		arglvlc[arglvl-1] --;
		if (arglvlc[arglvl-1] < arglvlo[arglvl-1])
			cerror ("arglvl: overrun2");
		arglvlc[arglvl-1] --;
		if (arglvlc[arglvl-1] < arglvlo[arglvl-1])
			cerror ("arglvl: overrun3");
		printf( "$B%05d+%d(8,%d)" , ftnno, arglvlc[arglvl-1]*4, STKREG );
		return;

	case 'D':
		arglvlc[arglvl-1] --;
		if (arglvlc[arglvl-1] < arglvlo[arglvl-1])
			cerror ("arglvl: overrunD2");
		arglvlc[arglvl-1] --;
		if (arglvlc[arglvl-1] < arglvlo[arglvl-1])
			cerror ("arglvl: overrunD3");
		printf( "$B%05d+%d(,%d)" , ftnno, arglvlc[arglvl-1]*4, STKREG );
		return;

	case '4':
		cerror ("Z4");
		return;

	case '~':
		/* complimented CR */
		p->right->lval = ~p->right->lval;
		adrput( getlr( p, 'R' ) );
		p->right->lval = ~p->right->lval;
		return;

	case 'M':
		/* negated CR */
		p->right->lval = -p->right->lval;
		conput( getlr( p, 'R' ) );
		p->right->lval = -p->right->lval;
		return;

	case 'L':  /* INIT for long constants */
		{
			unsigned hi, lo;
			lo = p->left->lval & BITMASK(SZINT);
			hi = ( p->left->lval >> SZINT ) & BITMASK(SZINT);
			printf( "	%o; %o  ZL\n", hi, lo );
			return;
		}

	case 'T':
		/* Truncate longs for type conversions:
		    LONG|ULONG -> CHAR|UCHAR|INT|UNSIGNED
		   increment offset to second word */

		m = p->type;
		p = p->left;
		switch( p->op ){
		case NAME:
		case OREG:
			p->lval += SZINT/SZCHAR;
			return;
		case REG:
			rfree( p->rval, p->type );
			p->rval += 1;
			p->type = m;
			rbusy( p->rval, p->type );
			return;
		default:
			cerror( "Illegal ZT type conversion" );
			return;

			}

	case 'U':
		/* same as AL for exp under U* */
		if( p->left->op == UNARY MUL ) {
			adrput( getlr( p->left, 'L' ) );
			return;
			}
		cerror( "Illegal ZU" );
		/* NO RETURN */

	case 'W':	/* structure size */
		if( p->op == STASG )
			printf( "%d", p->stsize);
		else	cerror( "Not a structure" );
		return;

	case 'a':
		ilfield = 1;
		return;

	case 'b':
		ilfield = 2;
		return;

	case 'c':
		ilfield = 4;
		return;

	case 'd':
		ilfield = 8;
		return;

	case 'i':	/* sconv int -> double */
		oreg_r = getlr( p, '3' )->rval;
		oreg_val = getlr( p, '3' )->lval;
		treg_r = getlr( p, '1' )->rval;
		freg_r = getlr( p, '2' )->rval;
		printf("         MVC   %d(4,%s),=XL8'4E00000080000000' sconv3i1\n", oreg_val, rnames[ oreg_r ]);
		printf("         ST    %s,%d(,%s)\n", rnames[ treg_r ], oreg_val+4, rnames[ oreg_r ]);
		printf("         XI    %d(%s),128\n", oreg_val+4, rnames[ oreg_r ]);
		printf("         LD    %s,%d(,%s)\n", rnames[ freg_r ], oreg_val, rnames[ oreg_r ]);
		printf("         SD    %s,=XL8'4E00000080000000'\n", rnames[ freg_r ]);
		return;

	case 'j':	/* sconv double -> int */
		oreg_r = getlr( p, '3' )->rval;
		oreg_val = getlr( p, '3' )->lval;
		treg_r = getlr( p, '1' )->rval;
		freg_r = getlr( p, '2' )->rval;
		printf("         LDR   %s," , rnames[ freg_r ]);
		adrput( getlr( p, 'L' ) );
		printf("    sconv5\n");
		printf("         AD    %s,=XL8'4F08000000000000'\n", rnames[ freg_r ]);
		printf("         STD   %s,%d(,%s)\n", rnames[ freg_r ], oreg_val, rnames[ oreg_r ]);
		printf("         L     %s,%d(,%s)\n", rnames[ treg_r ], oreg_val+4, rnames[ oreg_r ]);
		return;

	case 'k':
		if ( p->right->op != ICON )
			cerror ("Zk not ICON");
		printf ( "%ld", p->right->lval );
		return;

	case 'u':	/* sconv unsigned -> double */
#if 0
		printf("         L     2,0(11) sconv3u\n");
		printf("         MVC   80(4,13),=XL8'4E00000080000000'\n");
		printf("         ST    2,84(,13)\n");
		printf("         XI    84(13),128\n");
		printf("         LD    0,80(,13)\n");
		printf("         SD    0,=XL8'4E00000080000000'\n");
		printf("         STD   0,88(13)\n");
		printf("         L     2,0(11)\n");
		printf("         LTR   2,2\n");
		printf("         BNL   *+16   L2\n");
		printf("         LD    0,88(13)\n");
		printf("         AD    0,=D'4.294967296E9'\n");
		printf("         STD   0,88(13)\n");
		printf("*L2       EQU   *\n");
		printf("         LD    0,88(13)\n");
#else
		printf("         DC    X'0000'    FIXME sconv3u\n");
#endif
		return;

	case 'S':  /* structure assignment */
		{
			register NODE *l, *r;
			register size, count;

			if( p->op == STASG ){
				l = p->left;
				r = p->right;
				}
			else cerror( "STASG bad" );

			if( r->op == ICON ) r->op = NAME;
			else if( r->op == REG )
				r->op = OREG;
			else if( r->op != OREG ) cerror( "STASG-r" );

			size = p->stsize;
			count = size >> 2;

			if ( l->op == OREG ) {
				printf( "          L    " );
				adrput( getlr ( p, '1' ));
				printf (",");
				adrput( getlr ( r, 'R' ));
				printf( "    stasg1\n");
				printf("          MVC   %ld(%d,%s),0(%d)   stasg1a\n", l->lval, size, rnames[l->rval], getlr(p, '1')->rval);
				}
			 else {
				printf( "          L    " );
				adrput( getlr ( p, '1' ));
				printf (",");
				adrput( getlr ( l, 'L' ));
				printf( "    stasg2\n");
				printf("          MVC   0(%d,", size);
				adrput( getlr ( p, '1' ));
				printf ("),"),
				adrput( getlr ( r, 'R' ));
				printf ("    stasg2a\n");
				/* %d(%d,%s),0(%d)   stasg2a\n", l->lval, size, rnames[l->rval], getlr(p, '1')->rval); */
				}

			if( r->op == NAME ) r->op = ICON;
			else if( r->op == OREG ) r->op = REG;

			}
		break;

	default:
		cerror( "illegal zzzcode" );
		}
	}

rmove( rt, rs, t ) TWORD t; {
	printf( "          LR   %s,%s   rmove\n", rnames[rt], rnames[rs] );
	}

struct respref
respref[] = {
	INTAREG|INTBREG,	INTAREG|INTBREG,
	INAREG|INBREG,	INAREG|INBREG|SOREG|STARREG|SNAME|STARNM|SCON,
	INTEMP,	INTEMP,
	FORARG,	FORARG,
	INTAREG,	SOREG|SNAME,
	0,	0 };

setregs(){ /* set up temporary registers */
	register i;

	/* use any unused variable registers as scratch registers */
	fregs = maxtreg>=MINRVAR ? maxtreg + 1 : MINRVAR;
	if( xdebug ){
		/* -x changes number of free regs to 2, -xx to 3, etc. */
		if( (xdebug+1) < fregs ) fregs = xdebug+1;
		}
	/* NOTE: for pdp11 fregs <= 4 for float regs */
	if( fregs > 4 ) fregs = 4;
	for( i=MINRVAR; i<=MAXRVAR; i++ )
		rstatus[i] = i<fregs ? SAREG|STAREG : SAREG;
	}

szty(t) TWORD t; { /* size, in words, needed to hold thing of type t */
	/* really is the number of registers to hold type t */
	switch( t ) {

	case LONG:
	case ULONG:
		return( SZLONG/SZINT );

	default:
		return(1);

		}
	}

rewfld( p ) NODE *p; {
	return(1);
	}

callreg(p) NODE *p; {
	return( R15 );
	}

shltype( o, p ) NODE *p; {
	if( o == NAME|| o==REG || o == ICON || o == OREG ) return( 1 );
	return( o==UNARY MUL && shumul(p->left) );
	}

flshape( p ) register NODE *p; {
	register o = p->op;
	if( o==NAME || o==REG || o==ICON || o==OREG ) return( 1 );
	return( o==UNARY MUL && shumul(p->left)==STARNM );
	}

shtemp( p ) register NODE *p; {
	if( p->op == UNARY MUL ) p = p->left;
	if( p->op == REG || p->op == OREG ) return( !istreg( p->rval ) );
	return( p->op == NAME || p->op == ICON );
	}

spsz( t, v ) TWORD t; CONSZ v; {

	/* is v the size to increment something of type t */

	if( !ISPTR(t) ) return( 0 );
	t = DECREF(t);

	if( ISPTR(t) ) return( v == 2 );

	switch( t ){

	case UCHAR:
	case CHAR:
		return( v == 1 );

	case INT:
	case UNSIGNED:
		return( v == 2 );

	case FLOAT:
		return( v == 4 );

	case DOUBLE:
		return( v == 8 );
		}

	return( 0 );
	}

shumul( p ) register NODE *p; {
	register o;

	o = p->op;
	if( /* o == NAME || */ o == OREG || o == ICON ) return( STARNM );

	if( ( o == INCR || o == ASG MINUS ) &&
	    ( p->left->op == REG && p->right->op == ICON ) &&
	    p->right->name[0] == '\0' &&
	    spsz( p->left->type, p->right->lval ) )
		return( STARREG );

	return( 0 );
	}

adrcon( val ) CONSZ val; {
	printf( CONFMT, val );
	}

conput( p ) register NODE *p; {
	switch( p->op ){

	case ICON:
		if ( p->name[0] ) {
			markref( p->rval );
			/* if (p->name[0] == '$' && p->name[1] == 'L') */
			if ( p->lval )
				fprintf( outfile, "AL4(%.8s+%ld)", p->name, p->lval );
			else
				fprintf( outfile, "AL4(%.8s)", p->name );
			}
		else {
			fprintf ( outfile, "%c'%ld'", 'F', p->lval );
			}
		return;

	case REG:
		fprintf( outfile, "%s", rnames[p->rval] );
		return;

	default:
		cerror( "illegal conput" );
		}
	}

insput( p ) NODE *p; {
	cerror( "insput" );
	}

upput( p ) NODE *p; {
	/* output the address of the second word in the
	   pair pointed to by p (for LONGs)*/
	CONSZ save;

	if( p->op == FLD ){
		p = p->left;
		}

	save = p->lval;
	switch( p->op ){

	case NAME:
		p->lval += SZINT/SZCHAR;
		acon( p );
		break;

	case ICON:
		/* addressable value of the constant */
		p->lval &= BITMASK(SZINT);
		printf( "$" );
		acon( p );
		break;

	case REG:
		printf( "%s", rnames[p->rval+1] );
		break;

	case OREG:
		p->lval += SZINT/SZCHAR;
		if( p->lval != 0 || p->name[0] != '\0' ) acon( p );
		printf( "(%s)", rnames[p->rval] );
		break;

	default:
		cerror( "illegal upper address" );
		break;

		}
	p->lval = save;

	}

starr() {
	printf ("starr\n");
	}

adrput( p ) register NODE *p; {
	/* output an address, with offsets, from p */

	if( p->op == FLD ){
		p = p->left;
		}
	switch( p->op ){

	case NAME:
		acon( p );
		return;

	case ICON:
		/* addressable value of the constant */
#if 0
		if( szty( p->type ) == 2 ) {
			/* print the high order value */
			CONSZ save;
			save = p->lval;
			p->lval = ( p->lval >> SZINT ) & BITMASK(SZINT);
			printf( "$" );
			acon( p );
			p->lval = save;
			return;
			}
		printf( "$" );
#endif
		acon( p );
		return;

	case REG:
		printf( "%s", rnames[p->rval] );
		return;

	case OREG:
		acon( p );
		if (ilfield) {
			printf( "(%d,%s)", ilfield, rnames[p->rval] );
			ilfield = 0;
			}
		else
			printf( "(%s)", rnames[p->rval] );
		return;

	case UNARY MUL:
		/* STARNM or STARREG found */
		if( tshape(p, STARNM) ) {
			adrput( p->left);
			}
		else {	/* STARREG - really auto inc or dec */
			/* turn into OREG so replacement node will
			   reflect the value of the expression */
			register i;
			register NODE *q, *l;
			int r;

			starr();
			l = p->left;	/* INCR or ASG MINUS node */
			q = l->left;	/* REG node */
			printf("* changing %o from %s to OREG\n", p, opst[p->op]);
			p->op = OREG;
			p->rall = q->rall;
			p->lval = q->lval;
			p->rval = q->rval;
			for( i=0; i<NCHNAM; i++ )
				p->name[i] = q->name[i];
			if( l->op == INCR ) {
#if 0
				r = getlr( p, '1' )->rval;
				printf ("          l    %d,", r);
				adrput( p );
				printf ("   oreg incr1\n");
				printf ("          la   %d,1(,%d)  oreg incr1a\n", r, r);
				printf ("          st   %d,", r);
				adrput( p );
				printf ("   oreg incr1b\n");
				p->lval -= l->right->lval;
#endif
				if( q->op != REG ) cerror ("STARREG not REG");
				r = q->rval;
				printf ("   la   %d,1(,%d)  oreg incr1a\n", r, r);
				}
			else {	/* l->op == ASG MINUS */
				cerror ("STARREG ASG MINUS");
				printf( "-" );
				adrput( p );
				}
			tfree( l );
		}
		return;

	default:
		fprintf(stderr, "illegal address: op = %s\n", opst[p->op]);
		cerror( "illegal address" );
		return;

		}

	}
#if 0
printp8(s) char*s; {
	int i;
	for (i = 0; i < 8 && *s; i++, s++) {
		if (*s == '_')
			putchar('@');
		else
			putchar(*s);
		}
}
#endif
acon( p ) register NODE *p; { /* print out a constant */
	char szchar;
	long lval;

	szchar = 'F';
	if (p->op == ICON && p->name[0] && p->rval < 0) {
		if (p->lval)
			printf ("=AL4(%.8s+%ld)", p->name, p->lval);
		else
			printf ("=AL4(%.8s)", p->name);
		return;
		}
#if 0
	if (p->name[0] != 0){
		fprintf(stderr, "acon: op %s %.8s l %d r %d line %d\n", opst[p->op], p->name, p->lval, p->rval, lineno);
		isstatc( p );
		}
#endif
	if (p->op == OREG) {
		if (p->name[0]) 
			cerror( "OREG[name]: %.8s rval %d", p->name, p->rval );
		if ( halfword && p->rval == ARGREG )
			printf ("%ld", p->lval + 2);
		else
			printf ("%ld", p->lval);
		halfword = 0;
		return;
		}
	if ( ISPTR( p->type ) )
		szchar = 'F';
	else if (BTYPE(p->type) == CHAR)
#if 1
		szchar = 'X';
#else
		cerror("siz2");
#endif
	else if (BTYPE(p->type) == INT || p->type == UNSIGNED)
		szchar = 'F';
	else if (BTYPE(p->type) == LONG || p->type == ULONG)
		szchar = 'F';
	else if (BTYPE(p->type) == SHORT || p->type == USHORT)
		szchar = 'H';
	else if (BTYPE(p->type) == DOUBLE)
		szchar = 'D';
	else if (BTYPE(p->type) == UNIONTY) {
		if (dimtab[ p->csiz ] == SZCHAR)
			szchar = 'X';
		else if (dimtab[ p->csiz ] == SZINT)
			szchar = 'F';
		else if (dimtab[ p->csiz ] == SZSHORT)
			szchar = 'H';
		else
			cerror( "acon: union failed type %d", p->type );
		}
	else
		szchar = '?';
		/* cerror( "acon: failed type %d", p->type ); */

	lval = p->lval;
#ifndef ibm
	if (sizeof(long) == 8 && !ISUNSIGNED( p->type ) && lval & 0x80000000L)
		lval |= 0xffffffff00000000L;  /* sign extension hack */
#endif
	if( p->name[0] == '\0' ){	/* constant only */
		if ( szchar == 'X' )
			printf( "=X'%02lx'", lval & 0xff);
		else
			printf( "=%c'%d'", szchar, lval);
		}
	else /* if( p->lval == 0) */ {	/* name only */
#if 0
		if (p->name[0] == '$' || tstatic( p->rval ))
			printf( "=A(");
		else
#endif
		printf( "=A(");
		printf( exname( p->name ) );
		if (p->lval)
			printf( "+%ld", p->lval );
		putchar( ')');
		markref( p->rval );
		}
#if 0
	else {				/* name + offset */
		printf("%.8s+%ld", p->name, p->lval);
		}
#endif
	}

genscall( p, cookie ) register NODE *p; {
	/* structure valued call */
	return( gencall( p, cookie ) );
	}

gencall( p, cookie ) register NODE *p; {
	/* generate the call given by p */
	register temp;
	register m;
	register i;
	int argtot;

	arglvl ++;
	if (arglvl >= ARGLVLS)
		cerror("arglvl");

	if( p->right ) temp = argsize( p->right );
	else temp = 0;

	arglvlt[arglvl-1] = temp >> 2;
	if (arglvl == 1)
		arglvlo[arglvl-1] = 0;
	else
		arglvlo[arglvl-1] = arglvlo[arglvl-2] + arglvlt[arglvl-2];
	arglvlc[arglvl-1] = arglvlt[arglvl-1] + arglvlo[arglvl-1];
	argtot = 0;
	for ( i=0; i<arglvl; i++ )
		argtot += arglvlt[i];
	if (argtot > arglmax)
		arglmax = argtot;
        if (rdebug) {
		printf( "*   arglvl %d arglmax %d temp/4 %d\n", arglvl, arglmax, temp>>2 );
		printf ("gencall: level %d argsize=%d", arglvl, temp);
		printf("%d\t%d\n", arglvlo[arglvl-1], arglvlt[arglvl-1]);
		}

	if( p->right ){ /* generate args */
		genargs( p->right );
		}

	if( !shltype( p->left->op, p->left ) ) {
		order( p->left, INAREG|SOREG );
		}

	if (arglvlc[arglvl-1] != arglvlo[arglvl-1])
		cerror ("arglvl arg count error");
	if ( temp ) {
		/* printf("          LA    1,$B%05d+%d(,%d)   gencall1 baseoff %ld args %d l %d\n", ftnno, arglvlo[arglvl-1]*4, STKREG, baseoff >> 3, temp >> 2, arglvl); */
		printf("          L     1,=AL4($B%05d+%d)   gencall1 baseoff %ld args %d l %d\n", ftnno, arglvlo[arglvl-1]*4, baseoff >> 3, temp >> 2, arglvl);
		printf("          AR    1,%d   gencall1a\n", STKREG);
	} else {
		printf("          SLR   1,1   gencall2\n" );
	}
	p->op = UNARY CALL;
	m = match( p, INTAREG|INTBREG );
	if ( temp )
		popargs( temp );
	arglvl --;
	return(m != MDONE);
	}

popargs( size ) register size; {
	/* pop arguments from stack */
	}

char *
ccbranches[] = {
	"          BE    %s     line %d case %d\n",
	"          BNE   %s     line %d case %d\n",
	"          BNH   %s     line %d case %d\n",
	"          BL    %s     line %d case %d\n",
	"          BNL   %s     line %d case %d\n",
	"          BH    %s     line %d case %d\n",
	"          BNH   %s     line %d case %d\n",
	"          BL    %s     line %d case %d\n",
	"          BNL   %s     line %d case %d\n",
	"          BH    %s     line %d case %d\n",
	};

/*	long branch table

   This table, when indexed by a logical operator,
   selects a set of three logical conditions required
   to generate long comparisons and branches.  A zero
   entry indicates that no branch is required.
   E.G.:  The <= operator would generate:
	cmp	AL,AR
	jlt	lable	/ 1st entry LT -> lable
	jgt	1f	/ 2nd entry GT -> 1f
	cmp	UL,UR
	jlos	lable	/ 3rd entry ULE -> lable
   1:
 */

int lbranches[][3] = {
	/*EQ*/	0,	NE,	EQ,
	/*NE*/	NE,	0,	NE,
	/*LE*/	LT,	GT,	ULE,
	/*LT*/	LT,	GT,	ULT,
	/*GE*/	GT,	LT,	UGE,
	/*GT*/	GT,	LT,	UGT,
	/*ULE*/	ULT,	UGT,	ULE,
	/*ULT*/	ULT,	UGT,	ULT,
	/*UGE*/	UGT,	ULT,	UGE,
	/*UGT*/	UGT,	ULT,	UGT,
	};

/* logical relations when compared in reverse order (cmp R,L) */
extern short revrel[] ;

cbgen( o, lab, mode ) { /*   printf conditional and unconditional branches */
	register *plb;
	int lab1f;
	int t;

	if( o == 0 ) printf( "          B    $L%05d    cbgen1\n", lab );
	else	if( o > UGT ) cerror( "bad conditional branch: %s", opst[o] );
	else {
		switch( brcase ) {

		case 'A':
		case 'C':
			plb = lbranches[ o-EQ ];
			lab1f = getlab();
			expand( brnode, FORCC, brcase=='C' ? "\tcmp\tAL,AR\n" : "\ttst\tAR\n" );
			if( *plb != 0 )
				printf( ccbranches[*plb-EQ], lab, lineno, 1);
			if( *++plb != 0 )
				printf( ccbranches[*plb-EQ], lab1f, lineno, 2);
			expand( brnode, FORCC, brcase=='C' ? "\tcmp\tUL,UR\n" : "\ttst\tUR\n" );
			printf( ccbranches[*++plb-EQ], lab, lineno, 3);
			deflab( lab1f, 1 );
			reclaim( brnode, RNULL, 0 );
			break;

		default:
			if( mode=='F' ) o = revrel[ o-EQ ];
#if 1
			o = negrel[ o-EQ ];
			printf( ccbranches[o-EQ], /* t=getlab() */ "*+10", lineno, 4);
			printf("          L    14,=A($L%05d)   cbgen2\n", lab);
			printf("          BR   14   cbgen2a\n");
			/* deflab( t, 0 ); */
#else
			printf( ccbranches[o-EQ], lab , lineno, 4);
#endif
			break;
			}

		brcase = 0;
		brnode = 0;
		}
	}

nextcook( p, cookie ) NODE *p; {
	/* we have failed to match p with cookie; try another */
	if( cookie == FORREW ) return( 0 );  /* hopeless! */
	if( !(cookie&(INTAREG|INTBREG)) ) return( INTAREG|INTBREG );
	if( !(cookie&INTEMP) && asgop(p->op) ) return( INTEMP|INAREG|INTAREG|INTBREG|INBREG );
	return( FORREW );
	}

lastchance( p, cook ) NODE *p; {
	/* forget it! */
	return(0);
	}

struct functbl {
	int fop;
	TWORD ftype;
	char *func;
	} opfunc[] = {
	MUL,		LONG,	"lmul",
	DIV,		LONG,	"ldiv",
	MOD,		LONG,	"lrem",
	ASG MUL,	LONG,	"almul",
	ASG DIV,	LONG,	"aldiv",
	ASG MOD,	LONG,	"alrem",
	MUL,		ULONG,	"lmul",
	DIV,		ULONG,	"uldiv",
	MOD,		ULONG,	"ulrem",
	ASG MUL,	ULONG,	"almul",
	ASG DIV,	ULONG,	"auldiv",
	ASG MOD,	ULONG,	"aulrem",
	0,	0,	0 };

hardops(p)  register NODE *p; {
	/* change hard to do operators into function calls.
	   for pdp11 do long * / %	*/
	register NODE *q;
	register struct functbl *f;
	register o;
	register TWORD t;

	return;

	o = p->op;
	t = p->type;
	if( t!=LONG && t!=ULONG ) return;

	for( f=opfunc; f->fop; f++ ) {
		if( o==f->fop && t==f->ftype ) goto convert;
		}
	return;

	/* need address of left node for ASG OP */
	/* WARNING - this won't work for long in a REG */
	convert:
	if( asgop( o ) ) {
		switch( p->left->op ) {

		case UNARY MUL:	/* convert to address */
			p->left->op = FREE;
			p->left = p->left->left;
			break;

		case NAME:	/* convert to ICON pointer */
			p->left->op = ICON;
			p->left->type = INCREF( p->left->type );
			break;

		case OREG:	/* convert OREG to address */
			p->left->op = REG;
			p->left->type = INCREF( p->left->type );
			if( p->left->lval != 0 ) {
				q = talloc();
				q->op = PLUS;
				q->rall = NOPREF;
				q->type = p->left->type;
				q->left = p->left;
				q->right = talloc();

				q->right->op = ICON;
				q->right->rall = NOPREF;
				q->right->type = INT;
				q->right->name[0] = '\0';
				q->right->lval = p->left->lval;
				q->right->rval = 0;

				p->left->lval = 0;
				p->left = q;
				}
			break;

		default:
			cerror( "Bad address for hard ops" );
			/* NO RETURN */

			}
		}

	/* build comma op for args to function */
	q = talloc();
	q->op = CM;
	q->rall = NOPREF;
	q->type = INT;
	q->left = p->left;
	q->right = p->right;
	p->op = CALL;
	p->right = q;

	/* put function name in left node of call */
	p->left = q = talloc();
	q->op = ICON;
	q->rall = NOPREF;
	q->type = INCREF( FTN + p->type );
	strcpy( q->name, f->func );
	q->lval = 0;
	q->rval = 0;

	return;

	}

optim2( p ) register NODE *p; {
	/* do local tree transformations and optimizations */
	}

myreader(p) register NODE *p; {
	walkf( p, hardops );	/* convert ops to function calls */
	canon( p );		/* expands r-vals for fileds */
	walkf( p, optim2 );
	}

special( p, shape ) register NODE *p; {
	/* special shape matching routine */

	switch( shape ) {

	case SCCON:
		if( p->op == ICON && p->name[0]=='\0' && p->lval>= -128 && p->lval <=127 ) return( 1 );
		break;

	case SICON:
		if( p->op == ICON && p->name[0]=='\0' && p->lval>= 0 && p->lval <=32767 ) return( 1 );
		break;

	default:
		cerror( "bad special shape" );

		}

	return( 0 );
	}

# ifndef ONEPASS
main( argc, argv ) char *argv[]; {
	return( mainp2( argc, argv ) );
	}
# endif
