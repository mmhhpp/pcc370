/* y.tab.c (370 EBCDIC version) */
# line 2 "dd:input"
#include "apl.h"
/* #include <memory.h> */


int	vcount;
int	scount;
int	litflag;
int	nlexsym;
int	context;
char	*iline;
char	*ccharp, *ccharp2;
data	lnumb;				/* current label number */
char	*labcpp;			/* label prologue */
char	*labcpe;			/* label epilogue */
int	immedcmd;				/* immediate command number */

static char oline[OBJS];

# line 21 "dd:input"
typedef union  {
	char	*charptr;
	char	charval;
} YYSTYPE;
# define lex0 257
# define lex1 258
# define lex2 259
# define lex3 260
# define lex4 261
# define lex5 262
# define lex6 263
# define lpar 264
# define rpar 265
# define lbkt 266
# define rbkt 267
# define eol 268
# define unk 269
# define com 270
# define com0 271
# define quad 272
# define asg 273
# define null 274
# define dot 275
# define cln 276
# define semi 277
# define comnt 278
# define tran 279
# define strng 280
# define nam 281
# define numb 282
# define nfun 283
# define mfun 284
# define dfun 285
# define comexpr 286
# define comnam 287
# define comnull 288
# define comlist 289
# define dscal 290
# define mdscal 291
# define m 292
# define d 293
# define md 294
# define msub 295
# define mdsub 296
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 556 "dd:input"

#include "tab.c"
#include "lex.c"
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 29,
	266, 69,
	-2, 67,
-1, 31,
	266, 70,
	-2, 90,
-1, 32,
	270, 91,
	271, 91,
	275, 91,
	-2, 88,
-1, 56,
	276, 93,
	-2, 48,
-1, 57,
	276, 94,
	-2, 47,
-1, 58,
	276, 95,
	-2, 63,
-1, 76,
	275, 92,
	-2, 81,
-1, 80,
	266, 87,
	-2, 85,
-1, 82,
	266, 86,
	-2, 90,
	};
# define YYNPROD 97
# define YYLAST 301
short yyact[]={

  26,  33, 130,  41,  98,  42,  43,  44,  27,  60,
  61,  63,  62,  36, 126,  54,  24,  56,  36,  57,
  58,  44,  68, 103, 132,  26,  33,  32,  28,  49,
  35,  29,  31,  27, 133, 116, 127, 115,  97, 109,
  54,  24,  56,  36,  57,  58,  44,  96,  89,  90,
 104,  33,  32,  28,  66,  35,  29,  31,  26,  12,
  93, 122, 134,  87,  85, 118,  27,  14,  41,  95,
  42,  43,  44,  10,  24,  23,  36,  22,  20,  26,
  25,  67,  40, 101,  33,  32,  28,  27,  35,  29,
  31, 107,  86,  48,   9,  24,  23,  36,  22,  20,
  26,   1,  81,  51,  64,  33,  32,  28,  27,  35,
  29,  31,  70,  88,  71,  92,  24,  23,  36,  22,
  20,  26,  19,  18,  84,  59,  33,  32,  28,  27,
  35,  29,  31,  13,  52, 112,  69,  24,  23,  36,
  22,  20,  80,  78,  65,  79,  73,  33,  32,  28,
  83,  35,  29,  31,  99,  53,  16,  72,   8,  37,
  91,  21,  76,  32, 114,  77,  35, 117,  82,   2,
   3,   4,   5,   6,   7,  39, 123,  74,  34,  30,
  38,  17, 121,  55,  15, 100,  46,  47, 102, 110,
  11,  50,  45,   0, 105,  75, 135,   0,   0, 136,
   0, 111,   0, 113,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  94,   0,   0, 119,   0,
   0, 120,   0,   0,   0,   0,  55,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 106, 108,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 119,   0, 124,
 125,   0,   0,   0,   0,   0,   0, 129, 131,   0,
   0,   0,   0, 128,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  94 };
short yypact[]={

 -88,-1000,-206,-143,-278,-278,-278,-239,-1000,-277,
-164,-214,-1000,-255,-1000,-128,-143,-202,-1000,-203,
-1000,-222,-1000,-1000,-1000,-1000,-143,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-269,-1000,-1000,-1000,-1000,-213,
-1000,-1000,-1000,-1000,-1000,-230,-1000,-1000,-1000,-1000,
-264,-1000,-1000,-1000,-185,-253,-1000,-1000,-1000,-218,
-143,-278,-278,-1000,-1000,-229,-1000,-143,-1000,-143,
-1000,-203,-1000,-238,-240,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-143,-1000,-1000,-143,-1000,-203,
-1000,-204,-1000,-278,-278,-1000,-267,-1000,-1000,-1000,
-1000,-1000,-232,-1000,-1000,-1000,-1000,-278,-1000,-1000,
-1000,-255,-1000,-1000,-1000,-289,-289,-243,-1000,-1000,
-205,-1000,-1000,-1000,-278,-1000,-230,-1000,-1000,-1000,
-1000,-1000,-1000,-143,-1000,-1000,-1000 };
short yypgo[]={

   0, 180,  82, 192,  69, 191, 134, 103, 155, 190,
 189, 133, 184, 181, 178, 167,  65, 156, 136,  92,
 175,  67, 125, 124, 123, 122, 114, 113, 112, 102,
 179, 161, 101,  94,  93,  91,  81,  80 };
short yyr1[]={

   0,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   1,   1,   2,   3,   3,   3,   4,   4,  33,  22,
  22,  22,  22,  35,  35,  34,  34,  34,  34,   5,
   5,   6,   7,   7,   7,   8,   9,   9,  21,  10,
  10,  36,  11,  11,  11,  12,  12,  13,  13,  13,
  13,  13,  13,  37,  37,  14,  23,  15,  15,  16,
  16,  17,  17,  17,  17,  17,  24,  24,  24,  25,
  25,  18,  18,  18,  18,  18,  26,  19,  27,  27,
  28,  28,  28,  28,  28,  28,  29,  29,  30,  30,
  30,  31,  31,  20,  20,  20,  20 };
short yyr2[]={

   0,   2,   4,   3,   4,   2,   2,   2,   2,   2,
   3,   1,   2,   3,   2,   1,   3,   1,   1,   2,
   2,   2,   1,   2,   1,   1,   2,   2,   1,   1,
   2,   2,   1,   2,   3,   2,   1,   1,   3,   1,
   1,   1,   1,   2,   3,   1,   4,   1,   1,   1,
   1,   3,   1,   2,   1,   1,   1,   1,   3,   1,
   0,   1,   2,   1,   2,   3,   1,   1,   1,   1,
   1,   1,   2,   1,   3,   3,   1,   3,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1 };
short yychk[]={

-1000, -32, 257, 258, 259, 260, 261, 262,  -8, -33,
 279,  -9, 265, -11, -21, -12, -17, -13, -24, -25,
 284, -31, 283, 281, 280, -37, 264, 272, 292, 295,
 -30, 296, 291, 290, -14, 294, 282,  -8,  -1, -20,
  -2, 281, 283, 284, 285,  -3,  -1,  -1, -34, 268,
  -5,  -7,  -6,  -8, 279, -20, 281, 283, 284, -22,
 286, 287, 289, 288, 268, -11, 268, -36, 277, -18,
 -28, -26, 285, 274, -31, -30, 290, 293, 271, 273,
 270, -29, 296, -11, -23, 266, -19, 266, -27, 270,
 271, -11, -37, 273, -20,  -4, 277, 268, 268,  -7,
  -6, 268, -11, 276, 268, -11, -20, -35, -20, 268,
 -10, -11, -21, -11, -19, 275, 275, -15, -16, -11,
 -11, -19, 265,  -2, -20, -20, 281, 268, -20, -31,
 291, -31, 267, 277, 267,  -4, -16 };
short yydef[]={

   0,  -2,   0,   0,   0,   0,   0,   0,   1,   0,
   0,   0,  18,  36,  37,  42,   0,  45,  61,   0,
  63,   0,  47,  48,  49,  50,   0,  52,  66,  -2,
  68,  -2,  -2,  92,  54,  89,  55,   5,   6,  15,
  11,  93,  94,  95,  96,   0,   7,   8,   9,  25,
   0,  28,  29,  32,   0,   0,  -2,  -2,  -2,   0,
   0,   0,   0,  22,   3,   0,  35,   0,  41,   0,
  71,   0,  73,   0,   0,  80,  -2,  82,  83,  84,
  -2,  76,  -2,  43,  60,  56,  62,   0,  64,  78,
  79,   0,  53,   0,  14,  12,   0,  17,  26,  27,
  30,  33,   0,  31,   2,  19,  20,  21,  24,   4,
  38,  39,  40,  44,  72,   0,   0,   0,  57,  59,
   0,  65,  51,  10,  15,  13,   0,  34,  23,  74,
  91,  75,  46,  60,  77,  16,  58 };
#
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

int yydebug = 0; /* 1 for debugging */
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

			if( yydebug ) printf( "error recovery discards char %d\n", yychar );

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

		if( yydebug ) printf("reduce %d\n",yyn);
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 1:
# line 54 "dd:input"
{
		*ccharp = EOF;
		integ = lastCode(oline);
		if (integ != ASGN && integ != PRINT && integ != COMNT) *ccharp++ = PRINT;
		*ccharp++ = EOL;
	} break;
case 2:
# line 61 "dd:input"
{
		*ccharp++ = IMMED;
		*ccharp++ = yypvt[-1].charval;
	} break;
case 3:
# line 69 "dd:input"
{
		*ccharp++ = SICLR0;
	} break;
case 4:
# line 73 "dd:input"
{
		*ccharp++ = SICLR;
	} break;
case 10:
# line 103 "dd:input"
{
		switch(context) {

		case lex3:
			name(yyval.charptr, AUTO);
			/*
			 * see comments in ai.c/funcomp() concerning
			 * label processing.
			 */
			*ccharp++ = ELID;
			break;

		case lex4:
			ccharp2 = ccharp;
			*ccharp++ = EOL;
			name(yyval.charptr, RVAL);
			name(yyval.charptr, REST);
			invert(yypvt[-0].charptr, ccharp2);
		}
	} break;
case 11:
# line 124 "dd:input"
{
		if(context == lex3) *ccharp++ = ELID;
		if(context == lex4){
			*ccharp++ = EOL;	/* pop previous result */
			*ccharp++ = NILRET;	/* return empty result */
		}
	} break;
case 12:
# line 133 "dd:input"
{
		if(context == lex4) invert(yyval.charptr, yypvt[-0].charptr);
	} break;
case 13:
# line 139 "dd:input"
{
		yyval.charptr = ccharp;
		switch(context) {

		case lex2:
			name(yypvt[-1].charptr, DF);
			break;

		case lex3:
			name(yypvt[-0].charptr, ARG2);
			name(yypvt[-2].charptr, ARG1);
			break;

		case lex4:
			name(yypvt[-2].charptr, REST);
			name(yypvt[-0].charptr, REST);
		}
	} break;
case 14:
# line 158 "dd:input"
{
		yyval.charptr = ccharp;
		switch(context) {

		case lex2:
			name(yypvt[-1].charptr, MF);
			break;

		case lex3:
			name(yypvt[-0].charptr, ARG1);
			break;

		case lex4:
			name(yypvt[-0].charptr, REST);
		}
	} break;
case 15:
# line 175 "dd:input"
{
		if(context == lex2) name(yyval.charptr, NF);
		yyval.charptr = ccharp;
	} break;
case 16:
# line 181 "dd:input"
{
		yyval.charptr = yypvt[-0].charptr;
		switch(context) {

		case lex3:
			name(yypvt[-1].charptr, AUTO);
			break;

		case lex4:
			ccharp2 = name(yypvt[-1].charptr, REST);
			invert(yyval.charptr, ccharp2);
		}
	} break;
case 17:
# line 195 "dd:input"
{
		yyval.charptr = ccharp;
	} break;
case 18:
# line 204 "dd:input"
{
		litflag = -1;
	} break;
case 20:
# line 210 "dd:input"
{
		name(yypvt[-0].charptr, NAME);
	} break;
case 23:
# line 218 "dd:input"
{
	    *ccharp++ = IMMED;
	    *ccharp++ = immedcmd;
	    name(yypvt[-0].charptr, NAME);
	} break;
case 24:
# line 224 "dd:input"
{
	    name(yypvt[-0].charptr, NAME);
	} break;
case 25:
# line 238 "dd:input"
{
		*ccharp++ = COMNT;
	} break;
case 26:
# line 242 "dd:input"
{
		*ccharp++ = COMNT;
	} break;
case 31:
# line 254 "dd:input"
{
		if(labgen) genlab(yypvt[-1].charptr);
	} break;
case 32:
# line 260 "dd:input"
{
		*ccharp = EOF;
		integ = lastCode(oline);
		if (integ != ASGN && integ != PRINT && integ != COMNT) *ccharp++ = PRINT;
	} break;
case 33:
# line 266 "dd:input"
{
		yyval.charptr = ccharp;
		*ccharp++ = BRAN0;
	} break;
case 34:
# line 271 "dd:input"
{
		yyval.charptr = yypvt[-1].charptr;
		*ccharp++ = BRAN;
	} break;
case 39:
# line 284 "dd:input"
{
		*ccharp++ = PRINT;
	} break;
case 41:
# line 290 "dd:input"
{
		*ccharp++ = HPRINT;
	} break;
case 43:
# line 296 "dd:input"
{
		invert(yyval.charptr, yypvt[-0].charptr);
	} break;
case 44:
# line 300 "dd:input"
{
		invert(yyval.charptr, yypvt[-0].charptr);
	} break;
case 46:
# line 306 "dd:input"
{
		invert(yyval.charptr, yypvt[-1].charptr);
		*ccharp++ = INDEX;
		*ccharp++ = scount;
		scount = yypvt[-2].charval;
	} break;
case 47:
# line 314 "dd:input"
{
		yyval.charptr = name(yyval.charptr, FUN);
	} break;
case 48:
# line 318 "dd:input"
{
		yyval.charptr = name(yyval.charptr, NAME);
	} break;
case 49:
# line 322 "dd:input"
{
		yyval.charptr = ccharp;
		ccharp += 2;
		integ = iline[-1];
		vcount = 0;
		for(;;) {
			if (*iline == '\n') {
				nlexsym = unk;
				break;
			}
			if (*iline == '\\') {
				char temp, sum;
				int dcount;
				iline++;
				switch (*iline) {
					case 'b':
					temp = '\b';
					break;

					case 't':
					temp = '\t';
					break;

					case 'n':
					temp = '\n';
					break;

					case 'e':
					temp = '\033';
					break;

					case 'f':
					temp = '\014';
					break;

					case 'r':
					temp = '\r';
					break;

					default:
					temp = *iline;
					if (temp == '\n') continue;
					if (isodigit(temp)) {
						dcount = 0;
						sum = 0;
						while (isodigit(temp) && dcount < 3) {
							sum = 8 * sum + (temp - '0');
							dcount++;
							iline++;
							temp = *iline;
						}
						if (temp == '\n') continue;
						temp = sum;
						iline--;
					}
					break;
				}
				*ccharp++ = temp;
				iline++;
				vcount++;
				continue;
			}
			if( *iline == integ) {
				iline++;
				if(*iline != integ) break;
			}
			*ccharp++ = *iline++;
			vcount++;
		}
		((struct chrstrct *)yyval.charptr)->c[0] = QUOT;
		((struct chrstrct *)yyval.charptr)->c[1] = vcount;
	} break;
case 50:
# line 395 "dd:input"
{
		*ccharp++ = CONST;
		*ccharp++ = vcount;
		invert(yyval.charptr, ccharp-2);
	} break;
case 51:
# line 401 "dd:input"
{
		yyval.charptr = yypvt[-1].charptr;
	} break;
case 52:
# line 405 "dd:input"
{
		yyval.charptr = ccharp;
		*ccharp++ = yypvt[-0].charval;
	} break;
case 53:
# line 411 "dd:input"
{
		vcount++;
	} break;
case 54:
# line 415 "dd:input"
{
		vcount = 1;
	} break;
case 55:
# line 420 "dd:input"
{
		yyval.charptr = ccharp;
		ccharp += copy(DA,&datum,ccharp,1);
	} break;
case 56:
# line 431 "dd:input"
{
		yyval.charval = scount;
		scount = 1;
	} break;
case 58:
# line 438 "dd:input"
{
		invert(yyval.charptr, yypvt[-0].charptr);
		scount++;
	} break;
case 60:
# line 444 "dd:input"
{
		yyval.charptr = ccharp;
		*ccharp++ = ELID;
	} break;
case 61:
# line 454 "dd:input"
{
		yyval.charptr = ccharp;
		*ccharp++ = yypvt[-0].charval;
	} break;
case 62:
# line 459 "dd:input"
{
		yyval.charptr = yypvt[-0].charptr;
		*ccharp++ = yypvt[-1].charval+1;
	} break;
case 63:
# line 464 "dd:input"
{
		yyval.charptr = name(yyval.charptr, FUN);
	} break;
case 64:
# line 468 "dd:input"
{
		yyval.charptr = ccharp;
		*ccharp++ = yypvt[-0].charval+1;
		*ccharp++ = yypvt[-1].charval;
	} break;
case 65:
# line 474 "dd:input"
{
		yyval.charptr = yypvt[-0].charptr;
		*ccharp++ = yypvt[-1].charval+3;
		*ccharp++ = yypvt[-2].charval;
	} break;
case 68:
# line 483 "dd:input"
{
		yyval.charval++;
	} break;
case 70:
# line 489 "dd:input"
{
		yyval.charval += 2;
	} break;
case 71:
# line 498 "dd:input"
{
		yyval.charptr = ccharp;
		*ccharp++ = yypvt[-0].charval;
	} break;
case 72:
# line 503 "dd:input"
{
		yyval.charptr = yypvt[-0].charptr;
		*ccharp++ = yypvt[-1].charval;
	} break;
case 73:
# line 508 "dd:input"
{
		yyval.charptr = name(yyval.charptr, FUN);
	} break;
case 74:
# line 512 "dd:input"
{
		yyval.charptr = ccharp;
		*ccharp++ = OPROD;
		*ccharp++ = yypvt[-0].charval;
	} break;
case 75:
# line 518 "dd:input"
{
		yyval.charptr = ccharp;
		*ccharp++ = IPROD;
		*ccharp++ = yypvt[-2].charval;
		*ccharp++ = yypvt[-0].charval;
	} break;
case 76:
# line 526 "dd:input"
{
		yyval.charval += 2;
	} break;
case 77:
# line 537 "dd:input"
{
		yyval.charptr = yypvt[-1].charptr;
	} break;

		}
		goto yystack;  /* stack new state and value */

	}
