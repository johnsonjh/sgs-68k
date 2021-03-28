/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) xbyte_cdb.h: version 24.1 created on 10/28/91 at 17:55:26	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)xbyte_cdb.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
struct xbyte_sel_uniq {
	unchar flags;
	unchar reserved;
	unchar motion_thres;
	unchar recon_thres;
	unchar gap_thres;
};

/* flags definitons	*/
#define XB_NAL		0x01	/* not auto load */
#define	XB_PE		0x02	/* parity  enable */
#define XB_EBD		0x04	/* even byte disconnect */
#define XB_NBE		0x08	/* no busy enable	*/
#define XB_CT		0x80	/* p5 cartridge tape	*/

struct xbyte_mode_data {
	struct mode_head mh;
	struct blk_dscptr bdsp;
	struct xbyte_sel_uniq vdr;
};

typedef	struct xbyte_mode_data XBYTE_MODE_DATA;

#define	XB_MODE_DATA_LEN	sizeof(struct xbyte_mode_data)

/* control byte definitions */
#define	XB_SHORT	0x80	/* write short filemark */
