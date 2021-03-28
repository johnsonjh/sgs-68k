/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sgs.h: version 25.1 created on 12/2/91 at 16:48:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sgs.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#)sgs.h	2.1		*/

/*	The symbol MC68MAGIC is defined in filehdr.h	*/

#ifndef MAGIC
#define MAGIC	MC68MAGIC
#endif

/*define ISMAGIC(x) ((((unsigned short)x)==((unsigned short)MAGIC))||\
		    (((unsigned short)x)==((unsigned short) 0520))||\
		    (((unsigned short)x)==((unsigned short) 0521))||\
		    (((unsigned short)x)==((unsigned short) 0522))||\
		    (((unsigned short)x)==((unsigned short) 0523))||\
		    (((unsigned short)x)==((unsigned short) 0610))||\
		    (((unsigned short)x)==((unsigned short) 0615))||\
		    (((unsigned short)x)==((unsigned short) 0620))||\
		    (((unsigned short)x)==((unsigned short) 0625))||\
		    (((unsigned short)x)==((unsigned short) 0570))||\
		    (((unsigned short)x)==((unsigned short) 0575)))
*/
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


#define	SGSNAME	""
#define SGS ""
#define RELEASE "Release 5.2 for 68000/020"




/* ==================================================================== */
 /*     @(#)sgs.h	2.3     */

#define ALLISMAGIC(x)   ((((unsigned short)x)==(unsigned short)M68MIMAGIC) || \
			(((unsigned short)x)==(unsigned short)M68AWRMAGIC) || \
			(((unsigned short)x)==(unsigned short)A68020MAGIC) || \
			(((unsigned short)x)==(unsigned short)M68AROMAGIC) || \
			(((unsigned short)x)==(unsigned short)M68ASAMAGIC) || \
			(((unsigned short)x)==(unsigned short)OM68NWRMAGIC)|| \
			(((unsigned short)x)==(unsigned short)OM68NROMAGIC)|| \
			(((unsigned short)x)==(unsigned short)M68NWRMAGIC) || \
			(((unsigned short)x)==(unsigned short)A68020MAGIC) || \
			(((unsigned short)x)==(unsigned short)N68020MAGIC) || \
			(((unsigned short)x)==(unsigned short)X68020MAGIC) || \
			(((unsigned short)x)==(unsigned short)M68NROMAGIC))

#define ISMAGIC(x) ALLISMAGIC(x)

#ifdef ARTYPE
#define ISARCHIVE(x)	((((unsigned short)x)==(unsigned short)ARTYPE))
#define ALLBADMAGIC(x)	((((x)>>8) < 7) && !ALLISMAGIC(x) && !ISARCHIVE(x))
#define LOCBADMAGIC(x)	((((x)>>8) < 7) && !LOCISMAGIC(x) && !ISARCHIVE(x))
#else /* !ARTYPE */
#define ALLBADMAGIC(x)	((((x)>>8) < 7) && !ALLISMAGIC(x))
#define LOCBADMAGIC(x)	((((x)>>8) < 7) && !LOCISMAGIC(x))
#endif /* ARTYPE */

