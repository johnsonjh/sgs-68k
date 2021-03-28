/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) hlvl_macro.h: version 24.1 created on 10/28/91 at 17:52:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)hlvl_macro.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#define	splhlv()	splserv()

/* set a integer to  chars */
#define ITOC4(w, x, y, z, i)	\
				w=(i)>>24; \
			        x=(i)>>16; \
				y=(i)>>8; \
				z=(i)
				
#define ITOC3(x, y, z, i)	\
			        x=(i)>>16; \
				y=(i)>>8; \
				z=(i)

#define ITOC2(y, z, i)	\
				y=(i)>>8; \
				z=(i)

	/* The CAT macros combine muliple bytes into a single uint */
#define CAT2(hi, low)		((((unchar) (hi)) << 8) | (unchar) (low))
#define	CAT3(hi, med, low)	((((unchar) (hi)) << 16) | CAT2((med), (low)))
#define	CAT4(hi, mhi, mlo, low)	((((unchar) (hi)) << 24) |\
					 CAT3((mhi), (mlo), (low)))

#define MAXNODRVS	16	/* max number of drvs */
