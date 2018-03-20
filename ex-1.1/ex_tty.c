#
/*
 * Ex - a text editor
 * Bill Joy UCB September 1977
 */

#include "ex.h"
#include "ex_tty.h"

char	tspace[64], **Tspace_;
/* char	ttycap[]; */

int	COLUMNS	1000;
int	LINES	24;

setterm(type)
	char *type;
{

	Setterm_(type, 1);
}

Setterm_(type, printerr)
	char *type;
	int printerr;
{
	char buf[512], *cgoto();
	register int unknown;
	extern struct varbl varbls[];

	if (type[0] == 0)
		type = "un";
	unknown = 0;
	if (tgetent(buf, type) != 1) {
		unknown = printerr;
		strcpy(buf, "un|unknown:0:0:?");
	}
	TTY_ = buf[0] | (buf[1] << 8);
/*
	tgetmodes(MODES);
*/
	LINES = tgetnum("li");
	if (LINES <= 0)
		LINES = 24;
	varbls[WINDOW].vvalue = varbls[WINDOW].vdefault = LINES - 1;
	varbls[SCROLL].vvalue = varbls[SCROLL].vdefault = LINES / 2;
	COLUMNS = tgetnum("co");
	if (COLUMNS <= 20)
		COLUMNS = 1000;
/*
	IT = tgetflag("it");
*/
	CA = tgetflag("ca");
	if (cgoto()[0] == 'O')
		CA = 0;
	AM = tgetflag("am");
	BS = tgetflag("bs");
	OS = tgetflag("os");
	Tspace_ = tspace;
	CLEAR = Tgetstr_("cl");
	NDSPACE = Tgetstr_("nd");
	UPLINE = Tgetstr_("up");
/*
	AL = Tgetstr_("al");
	DL = Tgetstr_("dl");
*/
	CE = Tgetstr_("ce");
/*
	CD = Tgetstr_("cd");
*/
	strcpy(ttytype, longname(buf, type));
	termreset();
	if (unknown)
		error("%s: Unknown terminal type@- do \"!ttytype -n\" for a list of valid types", type);
}

longname(bp, def)
	register char *bp;
	char *def;
{
	register char *cp;

	while (*bp && *bp != ':' && *bp != '|')
		bp++;
	if (*bp == '|') {
		bp++;
		cp = bp;
		while (*cp && *cp != ':' && *cp != '|')
			cp++;
		*cp = 0;
		return (bp);
	}
	return (def);
}

Tgetstr_(cp)
	char *cp;
{

	tgetstr(cp, &Tspace_);
}
