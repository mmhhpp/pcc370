/*
 *	UNIX APL\11
 *
 *
 *	UNIX APL was originally written by Ken Thompson at Bell Labs.
 *	It spent some time at Yale and finally arrived at Purdue
 *	University.  Since 1976 it has been modified by Jim Besemer
 *	and John Bruner at the School of Electrical Engineering, Purdue,
 *	under the direction of Dr. Anthony P. Reeves.  Mike Cain got a
 *  copy from John and used it extensively.  Many years later, Mike
 *  was motivated to clean the source up to be more or less portable.
 *  Mike currently maintains this version.
 */

#include "apl.h"

data	zero;
data	one;
data	pi;
data	maxexp;		/* the largest value such that exp(maxexp) is defined */
data	datum;
int		funtrace;	/* function trace enabled */
int		labgen;		/* label processing being done */
int		apl_term;	/* flag set if apl terminal mapping req'd */
jmp_buf	gbl_env;	/* Used for setexit/reset */
jmp_buf reset_env;
struct _thread thread;

struct item *stack[STKS], **sp;

struct nlist nlist[NLS];

struct si *gsip;

int		integ;
int		column;
int		intflg;
int		echoflg;
int		offexit;		/* if != 0, require ")off" to exit */
int		prwsflg;
int		ifile;
int		wfile;
int		debug;
int		ttystat[3];
long	startTime;
char	*pcp;			/* global copy of arg to exec */
int     rowsz;
int		mencflg;
int		aftrace;
char    *mencptr;
int		oldlb[MAXLAB];
int		pt;
int		syze;
int		pas1;
int		ibeam36;
int		protofile;
int		lastop;			/* last (current) operator exec'ed */
char	*scr_file;		/* scratch file name */
char	*ws_file;		/* apl workspace file */
int     lineNumber;


struct _idx idx;
