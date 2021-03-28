/*	%W%	*/
/*	assert.h	1.4	*/

#ident	"%Z%uts/head:%M%	%I%"

#ifdef NDEBUG
#define assert(EX)
#else
extern void _assert();
#define assert(EX) if (EX) ; else _assert("EX", __FILE__, __LINE__)
#endif
