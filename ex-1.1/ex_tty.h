/*
 * Ex - a text editor
 * Bill Joy UCB September 1977
 */

extern int	TTY_;

extern char	*CLEAR, *NDSPACE, *UPLINE, *HSTR, *CE;

extern char	CA, AM, BS, OS, PT, NOCR;
extern int	LINES, COLUMNS;

extern char	UPPERCASE;

extern char	ttytype[];

extern int	outcol, outline, destcol, destline;

/*
 * As yet unused capabilities...
 *
extern char	*AL, *DL, *CD;
extern char	IT;
extern int	MODES[2];
 */
