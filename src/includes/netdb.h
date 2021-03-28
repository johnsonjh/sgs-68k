/*
 * @(#) Copyright (c) 1985  The Wollongong Group, Inc.  All Rights Reserved
 */

#ident "@(#)netdb.h  (TWG)      1.3     89/06/27 "

/*
 * Structures returned by network
 * data base library.  All addresses
 * are supplied in host order, and
 * returned in network order (suitable
 * for use in system calls).
 */
struct	hostent {
	char	*h_name;	/* official name of host */
	char	**h_aliases;	/* alias list */
	int	h_addrtype;	/* host address type */
	int	h_length;	/* length of address */
	char 	**h_addr_list;
#define h_addr	h_addr_list[0]
};

#define HOST_NOT_FOUND	1 /* Authoritive Answer Host not found */
#define TRY_AGAIN	2 /* Non-Authoritive Host not found, or SERVERFAIL */
#define NO_RECOVERY	3 /* Non recoverable errors, FORMERR,REFUSED,NOTIMP */
#define NO_DATA		4 /* Valid host name, no address, look for MX record */
#define NO_ADDRESS NO_DATA

/*
 * Assumption here is that a network number
 * fits in 32 bits -- probably a poor one.
 */
struct	netent {
	char	*n_name;	/* official name of net */
	char	**n_aliases;	/* alias list */
	int	n_addrtype;	/* net address type */
	unsigned long	n_net;	/* network # */
};

struct	servent {
	char	*s_name;	/* official service name */
	char	**s_aliases;	/* alias list */
	int	s_port;		/* port # */
	char	*s_proto;	/* protocol to use */
};

struct	protoent {
	char	*p_name;	/* official protocol name */
	char	**p_aliases;	/* alias list */
	int	p_proto;	/* protocol # */
};

#ifdef SHORT_IDENT
#define gethostbyaddr Gethostbyaddr
#define gethostent GEthostent
#define getnetbyaddr Getnetbyaddr
#define getnetent GETnetent
#define getservbyport Getservbyport
#define getservent GEtservent
#define getprotobynumber Getprotobynumber
#define getprotoent GEtprotoent
#ifndef inet_netof
#define inet_netof Inet_netof	/* For collision look at <netinet/in.h> */
#endif /* inet_netof */
#endif /* SHORT_IDENT */

struct hostent	*gethostbyname(), *gethostbyaddr(), *gethostent();
struct netent	*getnetbyname(), *getnetbyaddr(), *getnetent();
struct servent	*getservbyname(), *getservbyport(), *getservent();
struct protoent	*getprotobyname(), *getprotobynumber(), *getprotoent();

#define DEV_TCP "/dev/tcp"
#define DEV_UDP "/dev/udp"
#define DEV_IP	"/dev/ip"
#define DEV_LOOP "/dev/loop"
#define DEV_RAW "/dev/raw"
#define DEV_SOMOD "/dev/somod"
#define DEV_UDD "/dev/udd"
#define DEV_UDS "/dev/uds"
#define DEV_NB "/dev/nb"

#define DEV_TCP_CTL	"/dev/tcp0"
#define DEV_UDP_CTL	"/dev/udp0"
#define DEV_IP_CTL	"/dev/ip0"
#define DEV_LOOP_CTL	"/dev/loop0"
#define DEV_RAW_CTL	"/dev/raw0"
#define DEV_SOMOD_CTL	"/dev/somod0"
#define DEV_UDD_CTL	"/dev/udd0"
#define DEV_UDS_CTL	"/dev/uds0"
#define DEV_NB_CTL	"/dev/nb0"

#define HOSTNAME	"/etc/HostName"
#define HOSTID		"/etc/HostId"
