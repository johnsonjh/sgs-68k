#ident	"@(#)modes.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* Pragma control of Compiler Mode (i.e. ANSI or "CUC") */

typedef struct s_modes {
	char *lxmch;			/* pragma keyword */
	short lxmval;			/* pragma value:  boolean */
} t_modes;

extern t_modes modes[];

#define mode_chk( modenum )	modes[ modenum ].lxmval

/* WARNING: this must correspond with the modes array, defined in scan.c */
enum {
	MODE_STRNULL,
	MODE_ASM,
	MODE_FLOAT,
	MODE_XBLOCK,
	MODE_TAGBLOCK,
	MODE_UNSIGNED,
	MODE_TOPDOWN,
	MODE_STRWRITE,
	MODE_ANSIKEYW,
	MODE_BENIGN,
};

