#ident	"@(#)sgs.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.1	89/06/08 00:37:27 root
	date and time created 89/06/08 00:37:27 by root

*/
/*	OLD MOTsgs.h	2.1		*/


enum { MODE_CUC, MODE_ANSI, MODE_STRICT_ANSI };	/* ANSI Conformance Modes */

/* bk: warning levels */
typedef enum {
	W_NONE,				/* no warnings */
	W_DFLT,				/* default warnings */
	W_STRICT,			/* strict warnings */
	W_LINT,				/* heavy-duty */
} warn_t;
#define W_LVLCHK(x) ( ((int) x >= (int) W_NONE) && ((int) x <= (int) W_LINT) ) 
#define W_LVLMAX	W_LINT
/* end of warning stuff */

/*	The symbol MC68MAGIC is defined in filehdr.h	*/

#ifndef MAGIC
#define MAGIC	A68020MAGIC
#endif
#define LIBMAGIC	0443

#ifdef ARTYPE	
#define ISARCHIVE(x)	((((unsigned short)x)==(unsigned short)ARTYPE))
#define BADMAGIC(x)	((((x)>>8) < 7) && !ISMAGIC(x) && !ISARCHIVE(x))
#else
#define BADMAGIC(x)	((((x)>>8) < 7) && !ISMAGIC(x))
#endif


/*
 *	When a UNIX aout header is to be built in the optional header,
 *	the following magic numbers can appear in that header:
 *
 *		AOUT1MAGIC : default : readonly sharable text segment
 *		AOUT2MAGIC:	     : writable text segment
 */

#define AOUT1MAGIC 0410
#define AOUT2MAGIC 0407
#define AOUT3MAGIC 0413
#define PAGEMAGIC	AOUT3MAGIC 


#define	SGSNAME	""
#define SGS ""
#define RELEASE "Arix C Compiler for System 90 OS Release 3.O"
#define CUC_RELEASE "Arix CUC Compiler for System 90 OS Release 3.O"




/* ==================================================================== */
 /*     OLD MOT:sgs.h	2.3     */
/* if this macro is for the linker, then don't recognize the 68000
 * Magic numbers. They are incompatible with the 68020 objects
 */
#ifdef LINKERMAGIC
#define M68KNUMS(x)	(0)
#else
#define M68KNUMS(x) \
	(	\
	(((unsigned short)x)==(unsigned short)M68MIMAGIC) || \
	(((unsigned short)x)==(unsigned short)M68AWRMAGIC) || \
	(((unsigned short)x)==(unsigned short)M68AROMAGIC) || \
	(((unsigned short)x)==(unsigned short)M68ASAMAGIC) || \
	(((unsigned short)x)==(unsigned short)OM68NWRMAGIC)|| \
	(((unsigned short)x)==(unsigned short)OM68NROMAGIC)|| \
	(((unsigned short)x)==(unsigned short)M68NROMAGIC) || \
	(((unsigned short)x)==(unsigned short)M68NWRMAGIC)  \
	)
#endif /* LINKERMAGIC */

#define M68020NUMS(x) \
	(	\
	(((unsigned short)x)==(unsigned short)A68020MAGIC) || \
	(((unsigned short)x)==(unsigned short)N68020MAGIC) || \
	(((unsigned short)x)==(unsigned short)X68020MAGIC)    \
	)

#define M68040NUMS(x) \
	(	\
	(((unsigned short)x)==(unsigned short)M68040MAGIC_OS) \
	)

#define ALLISMAGIC(x) ( M68KNUMS(x) || M68020NUMS(x) || M68040NUMS(x) )

#define ISMAGIC(x) ALLISMAGIC(x)

#ifdef ARTYPE
#define ISARCHIVE(x)	((((unsigned short)x)==(unsigned short)ARTYPE))
#define ALLBADMAGIC(x)	((((x)>>8) < 7) && !ALLISMAGIC(x) && !ISARCHIVE(x))
#define LOCBADMAGIC(x)	((((x)>>8) < 7) && !LOCISMAGIC(x) && !ISARCHIVE(x))
#else /* !ARTYPE */
#define ALLBADMAGIC(x)	((((x)>>8) < 7) && !ALLISMAGIC(x))
#define LOCBADMAGIC(x)	((((x)>>8) < 7) && !LOCISMAGIC(x))
#endif /* ARTYPE */

