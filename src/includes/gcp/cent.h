#ifndef SYS_GCP_CENT_H
#define SYS_GCP_CENT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cent.h: version 2.1 created on 5/22/89 at 19:06:03	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cent.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)head/gcp:cent.h	1.3"

#define CENT_DATA_STROBE	0x80	/* output */
#define CENT_ACKNLG		0x40	/* input */
#define CENT_BUSY		0x20	/* input */
#define CENT_PE			0x10	/* input. paper empty */
#define CENT_NOT_SLCT		0x08	/* input. not selected */
#define CENT_INPUT_PRIME	0x04	/* output. clear printer buffer */
#define CENT_FAULT		0x02	/* input */
#define CENT_PRESENT		0x01	/* input. future */

#define CENT_ERR		CENT_PE | CENT_NOT_SLCT | CENT_FAULT

#endif /* SYS_GCP_CENT_H */
