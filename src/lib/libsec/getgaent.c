/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) getgaent.c: version 25.1 created on 12/2/91 at 20:14:03	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)getgaent.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <stdio.h>
#include <sys/types.h>
#include <grp.h>
#include "auth.h"

#define ferror(p)       ((p)->_flag & _IOERR)

static	FILE	*ga_fp = NULL;

#if !defined(TRUE)
#define TRUE	1
#endif
#if !defined(FALSE)
#define FALSE	0
#endif


/* Adds entry to end of shadow group file, 0 = success, -1 = failure */
putgaent (shadow_file, tmp_ga)
FILE *shadow_file;
grpauth_t *tmp_ga;
{
	char **name_index;

	if (fseek(shadow_file, 0L, 1) < 0)
		return (-1);

	fprintf(shadow_file, "%s:%s:%u:%ld:",
			tmp_ga->ga_name, tmp_ga->ga_passwd,
		        tmp_ga->ga_gid,  tmp_ga->ga_acctstat);

	for (name_index = tmp_ga->ga_mem; *name_index != NULL; name_index++) {
		fputs(*name_index, shadow_file);
		if (*(name_index + 1) != NULL)
			putc(',', shadow_file);
	}
	putc('\n', shadow_file);

	return(ferror(shadow_file));
}


/* Opens shadow group file if not currently opened, seeks to beginning
   if already open */
void
setgaent ()
{
	if (ga_fp == NULL)
		ga_fp = fopen(SGRPFLE, "r");
	else
		rewind(ga_fp);
}


/* closes shadow group file */
void
endgaent ()
{
	if (ga_fp != NULL) {
		fclose(ga_fp);
		ga_fp = NULL;
	}
}


/* retrieves an entry from the shadow group file, NULL = failure, otherwise
   pointer to shadow group entry is returned */
grpauth_t *
getgaent ()
{
	static	grpauth_t ga;
	static  char buffer[BUFSIZ];
	static	char *group_members[BUFSIZ];
	char *p, *q, field_sep;
	int field_num, member_index = 0;
	extern char *strrchr(), *strchr();

	if (ga_fp == NULL) 
		setgaent();

	if (fgets(buffer, BUFSIZ, ga_fp) == NULL)
		return((grpauth_t *) NULL);

	if ((p = strrchr(buffer, '\n')) != NULL)
		*p = '\0';

	field_sep = ':';
	for (p = buffer, q = p, field_num = 0; q != NULL; p = q, field_num++)  {
		if ((q = strchr(p, field_sep)) != NULL)
			*q++ = '\0';

		switch (field_num) {
			case 0:		ga.ga_name = p;
					break;
			case 1:		ga.ga_passwd = p;
					break;
			case 2:		ga.ga_gid = atoi(p);
					break;
			case 3:		ga.ga_acctstat = atol(p);
					field_sep = ',';
					member_index = 0;
					break;
			default:	group_members[member_index++] = p;
					break;
		}
	}
	group_members[member_index] = NULL;
	ga.ga_mem = group_members;

	return (&ga);
}

/* Get a group auth struct given a group name */
grpauth_t *
getganam(group_name)
char *group_name;
{
	grpauth_t *ga;

	setgaent();
	while ((ga = getgaent()) != NULL)
		if (strcmp(ga->ga_name, group_name) == 0)
			return(ga);
	endgaent();
	return((grpauth_t *)NULL);
}

/* Get a group auth struct given a group ID */
grpauth_t *
getgagid(group_id)
int group_id;
{
	grpauth_t *ga;

	setgaent();
	while ((ga = getgaent()) != NULL) {
		if (ga->ga_gid == group_id)
			return(ga);
	}
	endgaent();
	return((grpauth_t *) NULL);
}



/* Updates currently existing shadow group file, searches for an entry
   which has the same name as an existing group, if it is found, it's updated,
   otherwise its added to the end of the shadow group */

int
updategaent(new_entry)
grpauth_t *new_entry;
{
	FILE *group_file, *temp_file;
	grpauth_t *group_entry;
	grpauth_t new_one;
	char buffer[BUFSIZ], *p;
	char big_buffer[2 * BUFSIZ], *name_list[BUFSIZ];
	char **name_index, **new_index;
	int found_entry = FALSE;

	/* Copy the new_entry into new memory because it is probably in
	 * the static memory used by getgaent().
	 */
	p = buffer;
	strcpy(p, new_entry->ga_name);
	new_one.ga_name = p;
	p += strlen(p) + 1;
	strcpy(p, new_entry->ga_passwd);
	new_one.ga_passwd = p;
	new_one.ga_gid = new_entry->ga_gid;
	new_one.ga_acctstat = new_entry->ga_acctstat;
	new_index = name_list;
	for (p = big_buffer,name_index = new_entry->ga_mem; *name_index != NULL;
								name_index++) {
		strcpy(p, *name_index);
		*new_index++ = p;
		p += strlen(p) + 1;
	}
	*new_index = NULL;
	new_one.ga_mem = name_list;

	/* Create and Open the temp file */
	if ((temp_file = fopen(SGRPTMP, "w")) == NULL)
		return(-1);

	/* read the real shadow group file writing the entries to the temp file
	 * until we locate our entry, then write our entry.
	 */
	setgaent();
	while ((group_entry = getgaent()) != NULL) {
		if (strcmp(group_entry->ga_name, new_one.ga_name) == 0) {
			putgaent(temp_file, &new_one);
			found_entry = TRUE;
		}
		else
			putgaent(temp_file, group_entry);
	}
	endgaent();

	/* If we didn't find it then add it to the bottom. */
	if (found_entry == FALSE)
		putgaent(temp_file, &new_one);
	fclose(temp_file);

	/* Replace the group file with the temp version (make a backup first) */
	if (rename(SGRPFLE, OSGRPFLE) < 0) {
		unlink(SGRPTMP);
		return(-1);
	}

	if (rename(SGRPTMP, SGRPFLE) < 0)
		return(-1);
	return(0);
}
