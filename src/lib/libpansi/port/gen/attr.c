#ident	"@(#)attr.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "sys/termio.h"
#include "sys/errno.h"

extern int errno;

int
tcgetattr(fd, termios_p)
int fd;
register struct termios *termios_p;
{
	struct termio temp;
	int i;

	if ( fd < 0 ){
		errno = EBADF;
		return (-1);
		}

	if(!isatty(fd)) {
		errno = ENOTTY;
		return(-1);
	}

	if( (ioctl(fd, TCGETA, &temp)) < 0 ) 
		return(-1);

	termios_p->c_iflag = (unsigned int)temp.c_iflag;
	termios_p->c_oflag = (unsigned int)temp.c_oflag;
	termios_p->c_cflag = (unsigned int)temp.c_cflag;
	termios_p->c_lflag = (unsigned int)temp.c_lflag;
	termios_p->c_line  = temp.c_iflag;
	for (i = 0; i < NCC; i++)
		termios_p->c_cc[i] = temp.c_cc[i];
	termios_p->c_cc[VSTOP] = CSTOP;
	termios_p->c_cc[VSTART] = CSTART;

	return(0);
}


int
tcsetattr(fd, optional_actions, termios_p)
int fd;
int optional_actions;
register struct termios *termios_p;
{
	struct termio temp;
	int i;

	switch ( optional_actions ) {
		case TCSANOW:
		case TCSADRAIN:
		case TCSAFLUSH:
			break;
		default:
			errno = EINVAL;
			return(-1);
	}

	if ( fd < 0 ) {
		errno = EBADF;
		return (-1);
	}

	if(!isatty(fd)) {
		errno = ENOTTY;
		return(-1);
	}

	if( optional_actions & TCSADRAIN ) {
		if( ioctl(fd, TCSBRK, 1) < 0 ) 
			return(-1);
	} else if( optional_actions & TCSADFLUSH ) {
		if( ioctl(fd, TCSBRK, 1) < 0 ) 
			return(-1);
		if( ioctl(fd, TCFLSH, 0) < 0 )
			return(-1);
	}

	temp.c_iflag = (unsigned short)termios_p->c_iflag;
	temp.c_oflag = (unsigned short)termios_p->c_oflag;
	temp.c_cflag = (unsigned short)termios_p->c_cflag;
	temp.c_lflag = (unsigned short)termios_p->c_lflag;
	temp.c_line  = termios_p->c_line;
	for (i = 0; i < NCC; i++)
		temp.c_cc[i] = termios_p->c_cc[i];

	if( (ioctl(fd, TCSETA, &temp)) < 0 ) 
		return(-1);

	return(0);
}
