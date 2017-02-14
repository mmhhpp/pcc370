# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>

# include "mfile1"

int proflag;
int strftn = 0; /* is the current function one which returns a value */
FILE *tempfile;
FILE *tempf2;
FILE *outfile; /*  = stdout; */
extern OFFSZ maxoff;
extern int arglmax;

extern expands;
extern pagno;
extern pages;

branch( n ){
 /* output a branch to label n */
 /* exception is an ordinary function branching to retlab: then, return */
 if( n == retlab && !strftn ){
  printf(" L 14,=A($R%05d)\n   br   14   branch1   line %d\n", ftnno, lineno);
  }
 else printf(" L 14,=A($L%05d)\n  br   14     branch2    line %d retlab %d\n", n, lineno, retlab );
 }

int lastloc = PROG;

defalign(n) {
 /* cause the alignment to become a multiple of n */
 n /= SZCHAR;
 if( lastloc != PROG && n > 1 ) {
  if( n == 4 ) fprintf (outfile, "          DS    0F    ds1\n");
  else if( n == 8 ) fprintf (outfile, "          DS    0D    ds2\n");
 }
 }

locctr( l ){
 register temp;
 /* l is PROG, ADATA, DATA, STRNG, ISTRNG, or STAB */

 if( l == lastloc ) return(l);
 temp = lastloc;
 lastloc = l;
 switch( l ){

 case PROG:
  outfile = stdout;
  break;

 case DATA:
 case ADATA:
  outfile = tempf2;
  break;

 case STRNG:
 case ISTRNG:
  outfile = tempfile;
  break;

 case STAB:
  cerror( "locctr: STAB unused" );
  break;

 default:
  cerror( "illegal location counter" );
  }

 return( temp );
 }

deflab( n, code ){
 /* output something to define the current position as label n */
 fprintf( outfile, "$L%05d  EQU  *     deflab line %d\n", n, lineno );
 if( code ) {
   fprintf( outfile, "          L   12,%d(,10)   deflab2 line %d\n", pagno << 2, lineno );
   expands ++;
   }
 }

int crslab = 10;

getlab(){
 /* return a number usable for a label */
 return( ++crslab );
 }

efcode(){
 /* code for the end of a function */
  int i;
#if 1
 if( strftn ){  /* copy output to caller */
  register struct symtab *p;
  register int stlab;
  register int count;
  int size;

  p = &stab[curftn];

  deflab( retlab, 1 );

  stlab = getlab();
  printf( "          l     14,=A($L%05d)   strftn1\n", stlab );
  size = tsize( DECREF(p->stype), p->dimoff, p->sizoff ) / SZCHAR;
  printf( "          mvc   0(%d,14),0(15)  strftn2\n", size );
  printf( "          lr    15,14     strftn3\n", stlab );
  fprintf(tempfile,  "          DS    0F\n" );
  fprintf(tempfile,  "$L%05d   DS   XL%d  strftn\n", stlab, size );
  /* turn off strftn flag, so return sequence will be generated */
  strftn = 0;
  }
#endif
 printf ("$R%05d  ds    0h   efcode1\n", ftnno);
 printf ("          PDPEPIL\n");
 printf ("          DS    0F\n");
 printf ("          LTORG\n");

 printf ("          DS    0F   efcode2   arglmax = %d\n", arglmax);
 printf ("$T%05d      EQU   *    efcode3 %d pages\n", ftnno, pagno+1);
 for ( i=0; i<=pagno; i++) {
    printf ("          DC    A($P%05d)   efcode3a %d %d\n", i+pages-pagno, pagno, i);
    }

 /* total autos, temps, and args */
 printf ("$F%05d EQU   %d   efcode4\n", ftnno, (maxoff >> 3) + (arglmax << 2));
 /* autos, temps */
 printf ("$B%05d EQU   %d   efcode5\n", ftnno, maxoff >> 3);
#if 0
 /* args */
 printf ("$A%05d EQU   %d   efcode6\n", ftnno, arglmax << 2);
#endif
 arglmax = 0;

 pagno = 0;
 pages++;
 
 p2bend();
 }

bfcode( a, n ) int a[]; {
 /* code for the beginning of a function; a is an array of
  indices in stab for the arguments; n is the number */
 register i;
 register temp;
 register struct symtab *p;
 int off;

 locctr( PROG );
 p = &stab[curftn];
 temp = p->stype;
 temp = DECREF(temp);
 strftn = (temp==STRTY) || (temp==UNIONTY);

 retlab = getlab();
#if 0
 if( proflag ){
  int plab;
  plab = getlab();
  printf( " mov $L%d,r0\n", plab );
  printf( " jsr pc,mcount\n" );
  printf( " .bss\nL%d: .=.+2\n .text\n", plab );
  }
#endif

 /* routine prolog */

 printf( "          DS    0F\n" );
 printf( "%.8s PDPPRLG CINDEX=%d,FRAME=$F%05d,BASER=12,ENTRY=YES\n", exname( p->sname ), ftnno, ftnno);
 printf( "          B   $E%05d\n", ftnno );
 printf( "          LTORG\n" );
 printf( "$E%05d  EQU  *\n", ftnno );
 printf( "          DROP 12\n" );
 printf( "          BALR 12,0\n" );
 printf( "          USING *,12\n" );
 printf( "$P%05d  EQU    *          \n", pages );
 printf( "          LR    11,1\n" );
 printf( "          L     10,=A($T%05d)\n", ftnno );

 off = ARGINIT;
 expands = 0;

 for( i=0; i<n; ++i ){
  p = &stab[a[i]];
  if( p->sclass == REGISTER ){
   temp = p->offset;  /* save register number */
   p->sclass = PARAM;  /* forget that it is a register */
   p->offset = NOOFFSET;
   oalloc( p, &off );
   printf( "          l     %d,%d(,11)    reg arg\n", temp, p->offset/SZCHAR );
   p->offset = temp;  /* remember register number */
   p->sclass = REGISTER;   /* remember that it is a register */
   }
  else {
   if( oalloc( p, &off ) ) cerror( "bad argument" );
   }

  }
 }

bccode(){ /* called just before the first executable statment */
  /* by now, the automatics and register variables are allocated */
 SETOFF( autooff, SZINT );
 /* set aside store area offset */
 printf ("          DS    0H       bccode autooff/8 %d regvar %d\n", autooff >> 3, regvar);
 p2bbeg( autooff, regvar );
 }

ejobcode( flag ){
 /* called just before final exit */
 /* flag is 1 if errors, 0 if none */
 register i;
 register struct symtab *p;
 for (i=0,p=stab; i< SYMTSZ; ++i,++p) {
  if ( p->stype == TNULL ) continue;
  /* fprintf ( stderr, "%d\t%d\t%o\t%d\t%d\t%.8s\n", i, p->stype & 0xf, p->stype & ~0xf, p->sclass, p->slevel, exname( p->sname ) );*/
  if( p->sclass == EXTERN && (p->sflags&SREF) && !(p->sflags&SDEF) )
   printf( "          EXTRN  %s   extrn1\n", exname( p->sname ) );
  }
 }

aobeg(){
 /* called before removing automatics from stab */
 }

aocode(p) struct symtab *p; {
 /* called when automatic p removed from stab */
 }

aoend(){
 /* called after removing all automatics from stab */
 }

defnam( p ) register struct symtab *p; {
 /* define the current location as the name p->sname */
#if 0
 if (p->sizoff == DOUBLE) fprintf( tempfile, "          DC   0D\n");
 else if (p->sizoff > SHORT) fprintf( tempfile, "          DC   0F\n");
#endif
 if( p->sclass == EXTDEF ) fprintf( outfile, "          ENTRY %s\n", exname( p->sname));
 if( p->sclass == STATIC && p->slevel>1 ) deflab( p->offset, 0 );
 else fprintf( outfile, "%s  EQU   *    defnam sizoff %d d %d t %d/0%o c %d l %d\n", exname( p->sname ) , p->sizoff, dimtab[p->sizoff] >> 3, p->stype & 0xf, p->stype & ~0xf, p->sclass, p->slevel);
 }

int
tstatic( rval ) {
	register struct symtab *p;
	if ( rval < 0 ) return 0;
	p = &stab[rval];
	if (p->sclass == STATIC) return 1;
	return 0;
	}

markref( rval ) {
	register struct symtab *p;
	if ( rval < 0 ) return;
	p = &stab[rval];
	p->sflags |= SREF;
	}

dumpsym( rval ) {
	register struct symtab *p;
	if ( rval < 0 ) return;
	p = &stab[rval];
	fprintf( stderr, "%d:dumpsym %s sizoff %d d %d t %d/0%o c %d l %d\n", lineno, exname( p->sname ) , p->sizoff, dimtab[p->sizoff] >> 3, p->stype & 0xf, p->stype & ~0xf, p->sclass, p->slevel);
	}

bycode( t, i ){
 /* put byte i+1 in a string */

 i &= 07;
 if( t < 0 ){ /* end of the string */
  if( i != 0 ) fprintf( outfile, "\n" );
  }

 else { /* stash byte t into string */
  if( i == 0 ) fprintf( outfile, " DC " );
  else fprintf( outfile, "," );
#if 0
  if (t < ' ' || t > 0x7f) fprintf ( outfile, "XL1'%02x'", t);
  else if (t == '\'') fprintf ( outfile, "CL1''''" );
  else if (t == '&') fprintf ( outfile, "CL1'&&'" );
  else fprintf ( outfile, "CL1'%c'", t );
#endif
  fprintf ( outfile, "XL1'%02x'", t & 0xff);
  if( i == 07 ) fprintf( outfile, "\n" );
  }
 }

zecode( n ){
 /* n integer words of zeros */
 OFFSZ temp;
 register i;

 if( n <= 0 ) return;
 fprintf( outfile, "          DC   %dF'0'   zecode1\n", n );
 temp = n;
 inoff += temp*SZINT;
 }

fldal( t ) unsigned t; { /* return the alignment of field of type t */
 uerror( "illegal field type" );
 return( ALINT );
 }

fldty( p ) struct symtab *p; { /* fix up type of field p */
 ;
 }

where(c){ /* print location of error  */
 /* c is either 'u', 'c', or 'w' */
 fprintf( stderr, "%s, line %d: ", ftitle, lineno );
 }

char *tmpname = "/tmp/pccXXXXXX";
char *tmpn2 = "/tmp/pccX2XXXX";

main( argc, argv ) char *argv[]; {
 int dexit();
 register int c;
 register int i;
 int r;
 char tmpname_[20];
 char tmpn2_[20];

 outfile = stdout;
 for( i=1; i<argc; ++i )
  if( argv[i][0] == '-' && argv[i][1] == 'X' && argv[i][2] == 'p' ) {
   proflag = 1;
   }
#ifdef TARGET_370
 tempfile = fopen( "dd:sysut1", "w");
 tempf2 = fopen( "dd:sysut2", "w");
#else
 strcpy(tmpname_, tmpname);
 close(mkstemp(tmpname_));
 strcpy(tmpn2_, tmpn2);
 close(mkstemp(tmpn2_));
 tempfile = fopen( tmpname_, "w" );
 tempf2 = fopen( tmpn2_, "w" );
#endif
 if(signal( SIGHUP, SIG_IGN) != SIG_IGN) signal(SIGHUP, dexit);
 if(signal( SIGINT, SIG_IGN) != SIG_IGN) signal(SIGINT, dexit);
 if(signal( SIGTERM, SIG_IGN) != SIG_IGN) signal(SIGTERM, dexit);

 printf("          COPY  PDPTOP\n");
 printf("          CSECT\n");

 r = mainp1( argc, argv );

 tempfile = freopen( tmpname_, "r", tempfile );
 if( tempfile != NULL )
  while((c=getc(tempfile)) != EOF )
   putchar(c);
 else cerror( "Lost temp file" );
#ifndef TARGET_370
 unlink(tmpname_);
#endif
 tempf2 = freopen( tmpn2_, "r", tempf2 );
 if( tempf2 != NULL )
  while((c=getc(tempf2)) != EOF )
   putchar(c);
 else cerror( "Lost temp f2" );
#ifndef TARGET_370
 unlink(tmpn2_);
#endif
 printf("          END\n");
 return( r );
 }

dexit( v ) {
#ifndef TARGET_370
 unlink(tmpname);
#endif
 exit(1);
 }

genswitch(p,n) register struct sw *p;{
 /* p points to an array of structures, each consisting
  of a constant value and a label.
  The first is >=0 if there is a default label;
  its value is the label number
  The entries p[1] to p[n] are the nontrivial cases
  */
 register i;
 register CONSZ j, range;
 register dlab, swlab;

 range = p[n].sval-p[1].sval;

 if( range>0 && range <= 3*n && n>=4 ){ /* implement a direct switch */

  dlab = p->slab >= 0 ? p->slab : getlab();

  if( p[1].sval ){
   printf( "          S    15,=F'%d'  gensw0\n", p[1].sval );
   printf( "          BM   $L%05d  gensw01\n", dlab );
   }

  /* note that this is a cl; it thus checks
     for numbers below range as well as out of range.
     */
  printf( "          C    15,=F'%d'   gensw start\n", range );
  printf( "          BH   $L%05d\n", dlab );

  printf( "          SLL  15,2\n" );
  printf( "          L    14,=A($L%05d)\n", swlab = getlab() );
  printf( "          AR   14,15\n" );
  printf( "          L    14,0(,14)\n" );
  printf( "          BR   14   gensw end\n" );

  /* output table */

  locctr( ADATA );
  defalign( ALPOINT );
  deflab( swlab, 0 );

  for( i=1,j=p[1].sval; i<=n; ++j ){

   fprintf( outfile, "          DC   AL4($L%05d)\n", ( j == p[i].sval ) ?
    p[i++].slab : dlab );
   }

  locctr( PROG );

  if( p->slab< 0 ) deflab( dlab, 1 );
  return;

  }

 /* debugging code */

 /* out for the moment
 if( n >= 4 ) werror( "inefficient switch: %d, %d", n, (int) (range/n) );
 */

 /* simple switch code */

 for( i=1; i<=n; ++i ){
  /* already in r0 */

  printf( "          C    15,=F'%d'\n", p[i].sval );
  printf( "          BNE  *+10\n" );
  printf( "          L    14,=A($L%05d)\n", p[i].slab );
  printf( "          BR   14   gensw1\n" );
  }

 if( p->slab>=0 ) branch( p->slab );
 }
