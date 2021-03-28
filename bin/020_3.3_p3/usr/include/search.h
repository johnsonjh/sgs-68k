/*	search.h	1.1	*/


#ifndef SEARCH_H
#ident	"@(#)search.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define SEARCH_H

/* HSEARCH(3C) */
typedef struct entry { char *key, *data; } ENTRY;
typedef enum { FIND, ENTER } ACTION;

/* TSEARCH(3C) */
typedef enum { preorder, postorder, endorder, leaf } VISIT;

#endif /* SEARCH_H */
