/*	%W%	*/
/*	grp.h	1.1	*/

#ident	"%Z%uts/head:%M%	%I%"

/*	3.0 SID #	1.1	*/
struct	group {	/* see getgrent(3) */
	char	*gr_name;
	char	*gr_passwd;
	int	gr_gid;
	char	**gr_mem;
};
