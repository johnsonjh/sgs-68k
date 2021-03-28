#include	"curses_inc.h"

static	char	*keystrings[] =
		{
		    "BREAK",	/* 0401 */
		    "DOWN",	/* 0402 */
		    "UP",	/* 0403 */
		    "LEFT",	/* 0404 */
		    "RIGHT",	/* 0405 */
		    "HOME",	/* 0406 */
		    "BACKSPACE",	/* 0407 */
		    "DL",	/* 0510 */
		    "IL",	/* 0511 */
		    "DC",	/* 0512 */
		    "IC",	/* 0513 */
		    "EIC",	/* 0514 */
		    "CLEAR",	/* 0515 */
		    "EOS",	/* 0516 */
		    "EOL",	/* 0517 */
		    "SF",	/* 0520 */
		    "SR",	/* 0521 */
		    "NPAGE",	/* 0522 */
		    "PPAGE",	/* 0523 */
		    "STAB",	/* 0524 */
		    "CTAB",	/* 0525 */
		    "CATAB",	/* 0526 */
		    "ENTER",	/* 0527 */
		    "SRESET",	/* 0530 */
		    "RESET",	/* 0531 */
		    "PRINT",	/* 0532 */
		    "LL",	/* 0533 */
		    "A1",	/* 0534 */
		    "A3",	/* 0535 */
		    "B2",	/* 0536 */
		    "C1",	/* 0537 */
		    "C3",	/* 0540 */
		    "BTAB",	/* 0541 */
		    "BEG",	/* 0542 */
		    "CANCEL",	/* 0543 */
		    "CLOSE",	/* 0544 */
		    "COMMAND",	/* 0545 */
		    "COPY",	/* 0546 */
		    "CREATE",	/* 0547 */
		    "END",	/* 0550 */
		    "EXIT",	/* 0551 */
		    "FIND",	/* 0552 */
		    "HELP",	/* 0553 */
		    "MARK",	/* 0554 */
		    "MESSAGE",	/* 0555 */
		    "MOVE",	/* 0556 */
		    "NEXT",	/* 0557 */
		    "OPEN",	/* 0560 */
		    "OPTIONS",	/* 0561 */
		    "PREVIOUS",	/* 0562 */
		    "REDO",	/* 0563 */
		    "REFERENCE",	/* 0564 */
		    "REFRESH",	/* 0565 */
		    "REPLACE",	/* 0566 */
		    "RESTART",	/* 0567 */
		    "RESUME",	/* 0570 */
		    "SAVE",	/* 0571 */
		    "SBEG",	/* 0572 */
		    "SCANCEL",	/* 0573 */
		    "SCOMMAND",	/* 0574 */
		    "SCOPY",	/* 0575 */
		    "SCREATE",	/* 0576 */
		    "SDC",	/* 0577 */
		    "SDL",	/* 0600 */
		    "SELECT",	/* 0601 */
		    "SEND",	/* 0602 */
		    "SEOL",	/* 0603 */
		    "SEXIT",	/* 0604 */
		    "SFIND",	/* 0605 */
		    "SHELP",	/* 0606 */
		    "SHOME",	/* 0607 */
		    "SIC",	/* 0610 */
		    "SLEFT",	/* 0611 */
		    "SMESSAGE",	/* 0612 */
		    "SMOVE",	/* 0613 */
		    "SNEXT",	/* 0614 */
		    "SOPTIONS",	/* 0615 */
		    "SPREVIOUS",	/* 0616 */
		    "SPRINT",	/* 0617 */
		    "SREDO",	/* 0620 */
		    "SREPLACE",	/* 0621 */
		    "SRIGHT",	/* 0622 */
		    "SRSUME",	/* 0623 */
		    "SSAVE",	/* 0624 */
		    "SSUSPEND",	/* 0625 */
		    "SUNDO",	/* 0626 */
		    "SUSPEND",	/* 0627 */
		    "UNDO",	/* 0630 */
		};

char	*keyname(key)
int	key;
{
    static	char	buf[16];

    if (key >= 0400)
    {
	register	int	i;

	if ((key == 0400) || (key > KEY_UNDO))
	    return ("UNKNOWN KEY");
	if (key > 0507)
	    i = key - (0401 + ((0507 - 0410) + 1));
	else
	    if (key >= 0410)
	    {
		(void) sprintf(buf, "KEY_F(%d)", key - 0410);
		goto ret_buf;
	    }
	    else
		i = key - 0401;
	(void) sprintf(buf, "KEY_%s", keystrings[i]);
	goto ret_buf;
    }

    if (key >= 0200)
    {
	if (SHELLTTY.c_cflag & CS8)
	    (void) sprintf(buf, "%c", key);
	else
	    (void) sprintf(buf, "M-%s", unctrl(key & 0177));
	goto ret_buf;
    }

    if (key < 0)
    {
	(void) sprintf(buf, "%d", key);
ret_buf:
	return (buf);
    }

    return (unctrl(key));
}
