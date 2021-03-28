/*	aouthdr.h	2.4 12/15/82	*/


#ifndef AOUTHDR_H
#ident	"@(#)aouthdr.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define AOUTHDR_H

typedef	struct aouthdr {
	short	magic;		/* see magic.h				*/
	short	vstamp;		/* version stamp			*/
	long	tsize;		/* text size in bytes, padded to FW
				   bdry					*/
	long	dsize;		/* initialized data "  "		*/
	long	bsize;		/* uninitialized data "   "		*/
#if u3b
	long	dum1;
	long	dum2;		/* pad to entry point	*/
#endif
	long	entry;		/* entry pt.				*/
	long	text_start;	/* base of text used for this file	*/
	long	data_start;	/* base of data used for this file	*/
#ifdef SA_BOOTS
	struct {
		unsigned char bd_type;
		unsigned char bd_ifcard;
		unsigned char bd_iocard;
		unsigned char bd_opt;
		unsigned char user_opt;
		unsigned char boot_type;
		char boot_name[15];	/* rounded to 16 by ccom */
		unsigned short boothdr_version;
		long boothdrdummy[5];
		}boothdr;
#endif /*SA_BOOTS*/
} AOUTHDR;

#endif /* AOUTHDR_H */
