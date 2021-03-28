#ident	"@(#)tcsetpgrp.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

#include <sys/types.h>
#include <sys/termios.h>

tcgetpgrp(fildes)
int fildes;
{

	short group;

	if((ioctl(fildes,TCGETPGRP,&group)) < 0)
		return(-1);

	return(group);


}

tcsetpgrp(fildes,pgrp)
int fildes;
pid_t pgrp;
{

	return(ioctl(fildes,TCSETPGRP,&pgrp));

}

tcsendbreak(fildes,duration)
int fildes,duration;
{

	if(duration >= 1)
		duration++;
	return(ioctl(fildes,TCSBRK,duration));

}

tcdrain(fildes)
int fildes;
{

	return(ioctl(fildes,TCSBRK,1));

}

tcflush(fildes,queue_selector)
int fildes,queue_selector;
{

	return(ioctl(fildes,TCFLSH,queue_selector));

}

tcflow(fildes,action)
int fildes,action;
{

	return(ioctl(fildes,TCXONC,action));
}
