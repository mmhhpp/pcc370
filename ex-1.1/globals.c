/*
 * Ex - a text editor
 * Bill Joy UCB
 * Version 1.0 September, 1977
 */

#include "ex.h"
#include "ex_glob.h"
#include "ex_io.h"
#include "ex_re.h"
#include "ex_tty.h"
#include "ex_vis.h"

int gargc;

char	**xargv0, **xargv, **argv;
int	xargc0, xargc, argc;

char	ruptible, inglobal, inopen, inconf, listf, /* endline, */ laste, intty;
char	shudclob, diddle, die;

int	chngflag, xchngflag, tchngflag;

char	/* savedfile[FNS, */ file[FNSIZE], altfile[FNSIZE];
char	linebuf[LBSIZE], genbuf[LBSIZE];

int	*address(), *addr1, *addr2;
int	*zero, *one, *dot, *dol, *unddol, *endcore, *fendcore;
int	*unddel, *undap1, *undap2, *undadot;
char	undkind;

int	io, erfile, tfile;
char	*globp;
char	*erpath = "dd:strings";
int	names[27];
char	home[30];
char	*Command;

int	getfile(), gettty(), getchar(), getsub();

char	normtty;
int	normf;
#if 0
struct {
	int	fildes;
	int	nunused;
	char	*xfree;
	char	buff[512];
} obuf;
#else
char *obuf;
#endif
int	oldhup, onhup(), oldquit, onintr();

struct {
	int	Atime[2];
	int	Auid;
	int	Alines;
	int	Afname[FNSIZE];
	int	Ablocks[100];
} header;

int	dirtcnt;

char	recov;

int	TMODE;

char	aiflag;

int	pid, rpid, status, tty[3];
char	allredraw, pfast;
int	vcntcol;

int	errno;

struct savre_ {
	char	sexpbuf[ESIZE + 4];
	char	snbra;
	char	scircfl;
	char	*sbraslist[NBRA];
	char	*sbraelist[NBRA];
} re, scanre, subre;

char	rhsbuf[LBSIZE / 2];

char	*loc1, *loc2, *locs;
char	*linebp;

int	TTY_;

char	*CLEAR, *NDSPACE, *UPLINE, *HSTR, *CE;

char	CA, AM, BS, OS, PT, NOCR;

char	UPPERCASE;

int	outcol, outline, destcol, destline;

struct Glob *G, *G_;

#ifdef VISUAL
char	visual;
char	*cursor, *wcursor, *tcursor;
int	dir;

#ifdef vlast
#undef vlast
#endif
int	vlast;

char	vliny[TUBELINES + 2];
int	vcline, vcnt;

char	vch, *vtube[TUBELINES], *vtube0;

int	VLINES, VCOLUMNS;

char vmoving;
char vmovcol;

#endif

int	vdelete(), qcount(), vputchar();

#ifdef VISUAL

char	vholdmove;

char	holdat, splitw, hadup, *nextic;

char	vundkind, *vutmp;
int	vulines[TUBELINES], vylines[TUBELINES], *vresaddr;
int	vrestore(), vrescnt, vresCNT_, vdelcnt, vrescurs;
char	wdkind;
int	vSCROLL_;

int	(*Pline_)(), numbline(), normline();

char	Xhadcnt;
int	Xcnt;

char	lasthad;
int	lastcnt;

char	lastcmd[5], *lastcp, workcmd[5];
char	DEL[BUFSIZE], INS[BUFSIZE];

char	*vglobp;

char	HADUP_, HADZERO;
int	CDCNT;

int	ZERO_;

char	*ogcursor;
#endif
