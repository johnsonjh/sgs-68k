/*
 * @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ident "@(#)inetioctl.h  (TWG)      1.6     89/07/30 "

#ifndef SYS_IP_H
#include "sys/ip.h"
#endif

#define IPIOC			('I'<<8)
#define	IPIOC_SETIFADDR		(IPIOC|1)	/* Set interface Address */
#define IPIOC_GETIFADDR		(IPIOC|2)	/* Get Interface Addr	*/
#define IPIOC_SETIFFLAGS	(IPIOC|3)	/* Set interface flags	*/
#define IPIOC_GETIFFLAGS	(IPIOC|4)	/* Get interface Flags	*/
#define IPIOC_ADDROUTE		(IPIOC|5)	/* Add a route		*/
#define IPIOC_DELETEROUTE	(IPIOC|6)	/* Delete the route	*/
#define IPIOC_SARP		(IPIOC|7)	/* Set Arp table entry	*/
#define IPIOC_DARP		(IPIOC|8)	/* Delete Arp table entry */
#define IPIOC_GARP		(IPIOC|9)	/* Get Arp table entry	*/
#define IPIOC_SET_ARP_PROTO	(IPIOC|10)	/* Set Ethernet protocol*/
#define IPIOC_SETIFDSTADDR	(IPIOC|11)	/* Set Destination Address*/
#define IPIOC_GETIFDSTADDR	(IPIOC|12)	/* Get Destination Address*/
#define IPIOC_SETNAME		(IPIOC|13)	/* Set the interface name */
#define IPIOC_SETIFBRDADDR	(IPIOC|14)	/* Set broadcast addr	*/
#define IPIOC_GETIFBRDADDR	(IPIOC|15)	/* Get broadcast addr	*/
#define IPIOC_SETIFNETMASK	(IPIOC|16)	/* Set the subnetting masks */
#define IPIOC_GETIFNETMASK	(IPIOC|17)	/* Get the subnetting masks */
#define IPIOC_GETIFCONF		(IPIOC|18)	/* Get the interface conf.  */
#define IPIOC_SETIFMETRIC	(IPIOC|19)	/* Set the interface metric */
#define IPIOC_GETIFMETRIC	(IPIOC|20)	/* Get the interface metric */
#define IPIOC_GETIPFORWARD	(IPIOC|21)	/* Get the forwarding flag */
#define IPIOC_GETIPGATEWAY	(IPIOC|22)	/* Get the gateway flag */
#define IPIOC_GETIPREDIRECT	(IPIOC|23)	/* Get the redirect flag */
#define IPIOC_GETIPTTL		(IPIOC|24)	/* Get the ttl default */
#define IPIOC_SETIPFORWARD	(IPIOC|25)	/* Set the forwarding flag */
/*  note  values 30-39 reserved for ARP (see file arp.h) */
#define IPIOC_GETIPSTAT   	(IPIOC|40)	/* Get ipstat structure     */
#define IPIOC_GETICMPSTAT	(IPIOC|41)	/* Get icmpstat structure   */
#define IPIOC_GETRTSTAT   	(IPIOC|42)	/* Get rtstat structure     */
#define IPIOC_GETIPB    	(IPIOC|43)	/* Get ipb table entry      */
#define IPIOC_GETINADDR 	(IPIOC|44)	/* Get in_addr table entry  */
#define IPIOC_GETROUTE  	(IPIOC|45)	/* Get route table entry    */
#define IPIOC_GETPATHS  	(IPIOC|46)	/* Get paths to destination */
/* new values added above this line */
#define IPIOC_GETIFCOUNTS	(IPIOC|61)	/* Get the interface counts */

struct	tcpioctl {
	struct {
		ulong	tio_myinaddr;
		ushort	tio_myport;
	} Tio_Port;
};
#define tio_addr	Tio_Port.tio_myinaddr
#define tio_port	Tio_Port.tio_myport

/*
 * The TCP ioctl's that the user can use.
 */
#define TCPIOC			('T'<<8)	/* TCP Ioctl		*/
#define TCPIOC_GETMYPORT	(TCPIOC|1)	/* Find out binding addr*/
#define TCPIOC_DORDWR		(TCPIOC|2)	/* Don't send up M_PROTO */
#define TCPIOC_NORDWR		(TCPIOC|3)	/* Set M_PROTO		*/
#define TCPIOC_SPARE1		(TCPIOC|4)	/* Unused		*/
#define TCPIOC_GETSOCKNAME	(TCPIOC|5)	/* Get Sockname		*/
#define TCPIOC_GETPEERNAME	(TCPIOC|6)	/* Get Peer Name	*/
#define TCPIOC_SPARE2		(TCPIOC|7)	/* Unused		*/
#define TCPIOC_SETQLENGTH	(TCPIOC|8)	/* Set Queue Length 	*/
#define TCPIOC_GETMYSTATE       (TCPIOC|9)      /* Get the TCP STate    */
#define TCPIOC_DATAPEND         (TCPIOC|10)     /* Check the queue len  */
#define TCPIOC_NEWPORT          (TCPIOC|11)     /* Give me a new port   */
#define TCPIOC_SOCKOPT          (TCPIOC|12)     /* Handle socket options */
#define TCPIOC_GETFAMILY	(TCPIOC|13) /* AFUN - Get address family */
#define TCPIOC_SETFAMILY	(TCPIOC|14) /* AFUN - Set address family */
#define TCPIOC_GETPORT		(TCPIOC|15) /* AFUN - Get Port for afu file */
#define TCPIOC_GETTCPSTAT	(TCPIOC|16)     /* Get tcpstat structure */
#define TCPIOC_GETTCB   	(TCPIOC|17)     /* Get tcb, tcpcb, tcpd  */
#define TCPIOC_GETTOS		(TCPIOC|30)	/* Get Type of Service */
#define TCPIOC_SETTOS		(TCPIOC|31)	/* Set Type of Service */
#define TCPIOC_GETTTL		(TCPIOC|32)	/* Get Time to live */
#define TCPIOC_SETTTL		(TCPIOC|33)	/* Set Time to live */


/*
 * The UDP ioctl's that the user can use.
 */
#define UDPIOC			('U'<<8)	   /* UDP Ioctl		      */
#define UDPIOC_GETMYSTATE       TCPIOC_GETMYSTATE  /* Get the UDP's TLI state */
#define UDPIOC_GETSOCKNAME      TCPIOC_GETSOCKNAME /* Get the UDP's Sockname  */
#define UDPIOC_GETPEERNAME      TCPIOC_GETPEERNAME /* Get the UDP's Peer name */
#define UDPIOC_SOCKOPT          TCPIOC_SOCKOPT     /* Handle socket options   */
#define UDPIOC_CONNECT		SIOC_CONNECT	   /* Connect to peer's addr  */
#define UDPIOC_DO_CKSUM_IN	UDPIOC|2	   /* Checksum on input       */
#define UDPIOC_DO_CKSUM_OUT	UDPIOC|3	   /* Checksum on output      */
#define UDPIOC_DONT_CKSUM_IN	UDPIOC|4	   /* Don't checksum on input */
#define UDPIOC_DONT_CKSUM_OUT	UDPIOC|5	   /* Don't checksum on output*/
#define UDPIOC_GETFAMILY	TCPIOC_GETFAMILY   /* AFUN Get address family */
#define UDPIOC_SETFAMILY	TCPIOC_SETFAMILY   /* AFUN Set address family */
#define UDPIOC_GETPORT		TCPIOC_GETPORT  /* AFUN Get Port for afu file */
#define	UDPIOC_GETFILE		UDPIOC|6    /* AFUN Get filename knowing port */
#define	UDPIOC_GETUDPD		(UDPIOC|7)	   /* Get udpd table entry    */
#define	UDPIOC_GETUDPSTAT	(UDPIOC|8)	   /* Get udpstat structure   */
#define UDPIOC_GETTOS		(UDPIOC|30)	   /* Get Type of Service */
#define UDPIOC_SETTOS		(UDPIOC|31)	   /* Set Type of Service */
#define UDPIOC_GETTTL		(UDPIOC|32)	   /* Get Time to live */
#define UDPIOC_SETTTL		(UDPIOC|33)	   /* Set Time to live */

/*
 * The RAW ioctl's that the user can use.
 */
#define RAWIOC			('R'<<8)	   /* RAW Ioctl		*/
#define RAWIOC_GETMYSTATE       TCPIOC_GETMYSTATE  /* Get the RAW's TLI state */
#define RAWIOC_GETSOCKNAME      TCPIOC_GETSOCKNAME /* Get the RAW's Sockname  */
#define RAWIOC_GETPEERNAME      TCPIOC_GETPEERNAME /* Get the RAW's Peer name */
#define RAWIOC_SOCKOPT          TCPIOC_SOCKOPT     /* Handle socket options   */
#define RAWIOC_PROTO     	RAWIOC|1      /* Set RAW's proto # and family */
#define RAWIOC_CONNECT     	SIOC_CONNECT     /* Connect to peer's address */
#define RAWIOC_GETFAMILY	TCPIOC_GETFAMILY /* AFUN - Get address family */
#define RAWIOC_SETFAMILY	TCPIOC_SETFAMILY /* AFUN - Set address family */
#define RAWIOC_GETTOS		(RAWIOC|30)	   /* Get Type of Service */
#define RAWIOC_SETTOS		(RAWIOC|31)	   /* Set Type of Service */
#define RAWIOC_GETTTL		(RAWIOC|32)	   /* Get Time to live */
#define RAWIOC_SETTTL		(RAWIOC|33)	   /* Set Time to live */

/* The UDS ioctl's */
#define UDSIOC			('A'<< 8)	/* UDS Ioctl		*/
#define UDSIOC_SETINODE		(UDSIOC|1)	/* Find out binding addr*/
#define UDSIOC_GETINODE		(UDSIOC|2)	/* Don't send up M_PROTO */
#define UDSIOC_NORDWR		(UDSIOC|3)	/* Set M_PROTO		*/
#define UDSIOC_PAIR		(UDSIOC|4)	/* Unused		*/
#define UDSIOC_GETSOCKNAME	TCPIOC_GETSOCKNAME /* Get Sockname	*/
#define UDSIOC_GETPEERNAME	TCPIOC_GETPEERNAME /* Get Peer Name	*/
#define UDSIOC_SPARE2		(UDSIOC|7)	/* Unused		*/
#define UDSIOC_SETQLENGTH	TCPIOC_SETQLENGTH /* Set Queue Length 	*/
#define UDSIOC_GETMYSTATE       TCPIOC_GETMYSTATE /* Get the UDS STate */
#define UDSIOC_DATAPEND         (UDSIOC|10)     /* Check the queue len  */
#define UDSIOC_NEWPORT          (UDSIOC|11)     /* Give me a new port   */
#define UDSIOC_SOCKOPT          TCPIOC_SOCKOPT  /* Handle socket options */

/* UDD ioctls */
#define UDDIOC			('B' << 8)
#define	UDDIOC_SETINODE		(UDDIOC|1)
#define	UDDIOC_GETINODE		(UDDIOC|2)
#define	UDDIOC_CONNECT		(UDDIOC|3)
#define UDDIOC_GETSOCKNAME	TCPIOC_GETSOCKNAME
#define UDDIOC_GETPEERNAME	TCPIOC_GETPEERNAME
#define UDDIOC_SOCKOPT          TCPIOC_SOCKOPT
#define UDDIOC_GETMYSTATE       TCPIOC_GETMYSTATE

/*
 * 4.2 ioctls
 */
#define	IOCPARM_MASK	0x7f		/* parameters must be < 128 bytes */
#define	IOC_VOID	0x20000000	/* no parameters */
#define	IOC_OUT		0x40000000	/* copy out parameters */
#define	IOC_IN		0x80000000	/* copy in parameters */
#define	IOC_INOUT	(IOC_IN|IOC_OUT)
/* the 0x20000000 is so we can distinguish new ioctl's from old */
#define	_IO(x,y)	(IOC_VOID|('x'<<8)|y)
#define	_IOR(x,y,t)	(IOC_OUT|((sizeof(t)&IOCPARM_MASK)<<16)|('x'<<8)|y)
#define	_IOW(x,y,t)	(IOC_IN|((sizeof(t)&IOCPARM_MASK)<<16)|('x'<<8)|y)
/* this should be _IORW, but stdio got there first */
#define	_IOWR(x,y,t)	(IOC_INOUT|((sizeof(t)&IOCPARM_MASK)<<16)|('x'<<8)|y)

#define	TIOCREMOTE	_IO(t, 105)		/* remote input editing */
#define	TIOCPKT		_IOW(t, 112, int)	/* pty: set/clear packet mode */
#define		TIOCPKT_DATA		0x00	/* data packet */
#define		TIOCPKT_FLUSHREAD	0x01	/* flush packet */
#define		TIOCPKT_FLUSHWRITE	0x02	/* flush packet */
#define		TIOCPKT_STOP		0x04	/* stop output */
#define		TIOCPKT_START		0x08	/* start output */
#define		TIOCPKT_NOSTOP		0x10	/* no more ^S, ^Q */
#define		TIOCPKT_DOSTOP		0x20	/* now do ^S ^Q */

#define	FIONREAD	_IOR(f, 127, int)	/* get # bytes to read */
#define	FIONBIO		_IOW(f, 126, int)	/* set/clear non-blocking i/o */
#define	FIOASYNC	_IOW(f, 125, int)	/* set/clear async i/o */
#define	FIOSETOWN	_IOW(f, 124, int)	/* set owner */
#define	FIOGETOWN	_IOR(f, 123, int)	/* get owner */

#ifndef FIONCLEX
#define	FIONCLEX	_IO(f, 1)		/* Dont close on exec */
#define	FIOCLEX		_IO(f, 2)		/* close on exec */
#endif /* FIONCLEX */

/*
 * translate BSD ioctl commands to twg ioctl commands.
 */
#define SIOCGIFCONF	IPIOC_GETIFCONF
#define SIOCGIFFLAGS	IPIOC_GETIFFLAGS
#define SIOCGIFDSTADDR	IPIOC_GETIFDSTADDR
#define SIOCGIFBRDADDR	IPIOC_GETIFBRDADDR
#define SIOCGIFMETRIC	IPIOC_GETIFMETRIC
#define SIOCGIFNETMASK	IPIOC_GETIFNETMASK
#define SIOCGIFADDR	IPIOC_GETIFADDR
#define SIOCADDRT	IPIOC_ADDROUTE
#define SIOCDELRT	IPIOC_DELETEROUTE

#ifdef notdef
/* socket i/o controls */
#define	SIOCSHIWAT	_IOW(s,  0, int)		/* set high watermark */
#define	SIOCGHIWAT	_IOR(s,  1, int)		/* get high watermark */
#define	SIOCSLOWAT	_IOW(s,  2, int)		/* set low watermark */
#define	SIOCGLOWAT	_IOR(s,  3, int)		/* get low watermark */
#define	SIOCATMARK	_IOR(s,  7, int)		/* at oob mark? */
#define	SIOCSPGRP	_IOW(s,  8, int)		/* set process group */
#define	SIOCGPGRP	_IOR(s,  9, int)		/* get process group */

#define	SIOCADDRT	_IOW(r, 10, struct route)	/* ADD route */
#define	SIOCDELRT	_IOW(r, 11, struct route)	/* delete route */

#define	SIOCSIFADDR	_IOW(i, 12, struct ifreq)	/* set ifnet address */
#define	SIOCGIFADDR	_IOWR(i,13, struct ifreq)	/* get ifnet address */
#define	SIOCSIFDSTADDR	_IOW(i, 14, struct ifreq)	/* set p-p address */
#define	SIOCGIFDSTADDR	_IOWR(i,15, struct ifreq)	/* get p-p address */
#define	SIOCSIFFLAGS	_IOW(i, 16, struct ifreq)	/* set ifnet flags */
#define	SIOCGIFFLAGS	_IOWR(i,17, struct ifreq)	/* get ifnet flags */
#define	SIOCGIFBRDADDR	_IOWR(i,18, struct ifreq)	/* get broadcast addr */
#define	SIOCGIFCONF	_IOWR(i,20, struct ifconf)	/* get ifnet list */
#define	SIOCSIFENADDR	_IOW(i, 21, struct ifreq)	/* set ethernet addr */
#define	SIOCGIFENADDR	_IOWR(i,22, struct ifreq)	/* get ethernet addr */
#define SIOCSEPIF	_IOW(i, 23, struct epcontroller)/* set raw ether */
#define SIOCGEPIF	_IOWR(i,24, struct epcontroller)/* get raw ether */

#define	SIOCSARP	_IOW(i, 30, struct arpreq)	/* set arp entry */
#define	SIOCGARP	_IOWR(i,31, struct arpreq)	/* get arp entry */
#define	SIOCDARP	_IOW(i, 32, struct arpreq)	/* delete arp entry */
#endif /* notdef */

#define MAX(a,b) 	((a) < (b) ? b : a)
#define MIN(a,b) 	((a) > (b) ? b : a)
