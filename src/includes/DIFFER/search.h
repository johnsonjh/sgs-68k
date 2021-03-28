/*	%W%	*/
/*	search.h	1.1	*/

#ident	"%Z%uts/head:%M%	%I%"

/* HSEARCH(3C) */
typedef struct entry { char *key, *data; } ENTRY;
typedef enum { FIND, ENTER } ACTION;

/* TSEARCH(3C) */
typedef enum { preorder, postorder, endorder, leaf } VISIT;
