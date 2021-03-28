#ident	"@(#)baud.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "sys/termio.h"

cfgetospeed(termios_p)
struct termios	*termios_p;
{
	return(termios_p->c_cflag & 0xf);
}

cfgetispeed(termios_p)
struct termios	*termios_p;
{
	return(termios_p->c_cflag & 0xf);
}

cfsetospeed(termios_p,speed)
struct termios	*termios_p;
speed_t speed;
{
	termios_p->c_cflag = (termios_p->c_cflag & ~0xf) | (speed & 0xf);
	
	return(0);
}

cfsetispeed(termios_p,speed)
struct termios	*termios_p;
speed_t speed;
{
	if (speed != B0)

		termios_p->c_cflag = (termios_p->c_cflag & ~0xf) | (speed & 0xf);

	return(0);
}

