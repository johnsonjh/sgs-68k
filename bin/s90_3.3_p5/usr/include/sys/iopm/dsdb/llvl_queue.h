/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) llvl_queue.h: version 24.1 created on 10/28/91 at 17:51:22	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)llvl_queue.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* head of queue */

struct link_list{
	struct link_list *next;
};

typedef struct link_list ITEM;

typedef struct {
	ITEM	*head;  
	ITEM	*tail; 
}Q_HDR;
