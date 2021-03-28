/*	Copyright (c) 1984, 1986, 1987, 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_DLPI_H_
#define _SYS_DLPI_H_

#ident	"@(#)head.sys:sys/dlpi.h	1.2"

/*
 * dlpi.h header for Data Link Provider Interface
 */

/*
 * This header fil ehas encoded the values so an existing driver
 * or user which has written with the Logical Link Interface (LLI)
 * can migrate to the DLPI interface in a binary compatible manner.
 * Any fields which require a specific format or value are flagged
 * with a comment containing the message LLI compatibility.
 */

/*
 * Primitives for Local Management Services
 */
#define	DL_INFO_REQ	0x00	/* Information Req, LLI compatibility */
#define	DL_INFO_ACK	0x03	/* Information Ack, LLI compatibility */
#define DL_ATTACH_REQ	0x0b	/* Attach a PPA */
#define DL_DETACH_REQ	0x0c	/* Detach a PPA */
#define DL_BIND_REQ	0x01	/* Bind dlsap address, LLI compatibility */
#define	DL_BIND_ACK	0x04	/* Dlsap address bound, LLI compatibility */
#define	DL_UNBIND_REQ	0x02	/* Unbind dlsap address, LLI compatibility */
#define	DL_OK_ACK	0x06	/* Success acknowledgment, LLI compatibility */
#define	DL_ERROR_ACK	0x05	/* Error acknowledgement, LLI compatibility */
#define DL_SUB_BIND_REQ	0x1b	/* Bind subsequent DLSAP address */
#define	DL_SUB_BIND_ACK	0x1c	/* Subsequent DLSAP address bound */

/*
 * Primitives for Connectionless Services
 */
#define	DL_UNITDATA_REQ	0x07	/* unit_data send request */
#define	DL_UNITDATA_IND	0x08	/* unitdata receive indication 		*/
#define	DL_UDERROR_IND	0x09	/* unitdata receive indication 		*/
#define	DL_UDQOS_REQ	0x0a	/* unitdata receive indication 		*/

/*
 *	Primitive Non-fatal error return codes
 */
#define DLBADSAP	0	/* bad LSAP selector */
#define DLBADADDR	1	/* bad address */
#define	DLACCES		2	/* inproper permissions 		 */
#define	DLOUTSTATE	3	/* Link layer interface out of state     */
#define	DLSYSERR	4	/* Unix system error 			 */

/*
 * DLPI media types supported
 */
#define	DL_CSMACD	0	/* CSMA/CD network (802.3)   */
#define	DL_TPB		1	/* Token Passing Bus (802.4) */
#define	DL_TPR		2	/* Token Ring Bus (802.5)    */
#define	DL_METRO	3	/* Metro Net (802.6)         */
#define	DL_ETHER	4	/* ETHERNET bus 	     */
#define	DL_HDLC		5	/* ISO HDLC 		     */
#define	DL_CHAR		6	/* Character sync, eg BISYNC */
#define	DL_CTCA		7	/* IBM Channel		     */

/*
 * DLPI provider service supported.
 * These must be allowed to be bitwise-OR for dl_service_mode in
 * DL_INFO_ACK
 */
#define	DL_CODLS	0x01	/* support for connection-oriented */
#define	DL_CLDLS	0x02	/* support for connectionless data link */

/*
 * DLPI provider style.
 * The DLPI provider style which determines whether a provider
 * requires a DL_ATTACH_REQ to inform the provider which PPA
 * user messages should be sent/received on.
 */
#define	DL_STYLE1	0x0500	/* PPA is implicitly bound by open(2) */
#define	DL_STYLE2	0x0501	/* PPA must be explicitly bound via ATTACH */

/*
 * Link-Layer current state definitions
 */
#define	DL_UNBND	0	/* LL not bound 	     */
#define	DL_WACK_B	1	/* LL waiting for bind ack   */
#define	DL_WACK_U	2	/* LL waiting for unbind ack */
#define	DL_IDLE		3	/* LL is active		     */


/*
 * The following structure definitions define the format of the 
 * streams message blocks used to define the Link-Layer Interface.
 */


/* 
 *	User Originated Primitives
 */

/* DL_INFO_REQ, M_PCPROTO type */
typedef struct {
	ulong	dl_primitive;
} dl_info_req_t;

/* DL_INFO_ACK, M_PCPROTO type */
typedef struct {
	ulong	dl_primitive;
	ulong	dl_max_sdu;
	ulong	dl_min_sdu;
	ulong	dl_addr_length;
	ulong	dl_mac_type;
	ulong	dl_reserved;
	ulong	dl_current_state;
	ulong	dl_max_idu;
	ulong	dl_service_mode;
	ulong	dl_qos_length;
	ulong	dl_qos_offset;
	ulong	dl_qos_range_length;
	ulong	dl_qos_offset_length;
	ulong	dl_provider_style;
	ulong	dl_addr_offset;
	ulong	dl_growth;
} dl_info_ack_t;

/* DL_ATTACH_REQ, M_PROTO type */
typedef struct {
	ulong	dl_primitive;
	ulong	dl_ppa;
} dl_attach_req_t;

/* DL_DETACH_REQ, M_PROTO type */
typedef struct {
	ulong	dl_primitive;
} dl_detach_req_t;

/* DL_BIND_REQ, M_PROTO type */
typedef struct {
	ulong	dl_primitive;
	ulong	dl_sap;
	ulong	dl_max_conind;
	ulong	dl_service_mode;
	ulong	dl_conn_mgmt;
} dl_bind_req_t;

/* DL_BIND_ACK, M_PCPROTO type */
typedef struct {
	ulong	dl_primitive;
	ulong	dl_sap;
	ulong	dl_addr_length;
	ulong	dl_addr_offset;
	ulong	dl_max_conind;
	ulong	dl_growth;
} dl_bind_ack_t;

/* DL_UNBIND_REQ, M_PROTO type */
typedef struct {
	ulong	dl_primitive;
} dl_unbind_req_t;

/* DL_OK_ACK, M_PCPROTO type */
typedef struct {
	ulong	dl_primitive;
	ulong	dl_correct_primitive;
} dl_ok_ack_t;

/* DL_ERROR_ACK, M_PCPROTO type */
typedef struct {
	ulong	dl_primitive;
	ulong	dl_error_primitive;
	ulong	dl_errno;
	ulong	dl_unix_error;		/* UNIX error code */
} dl_error_ack_t;

/* DL_UNITDATA_REQ, M_PROTO type, with M_DATA block(s) */
typedef struct {
	ulong	dl_primitive;
	ulong	dl_dest_addr_length;
	ulong	dl_dest_addr_offset;
	ulong	dl_reserved[2];
} dl_unitdata_req_t;

/* DL_UNITDATA_IND, M_PROTO type, with M_DATA block(s) */
typedef struct {
	ulong	dl_primitive;
	ulong	dl_dest_addr_length;
	ulong	dl_dest_addr_offset;
	ulong	dl_src_addr_length;
	ulong	dl_src_addr_offset;
	ulong	dl_reserved;
} dl_unitdata_ind_t;

/* DL_UDERROR_IND, M_PROTO type, or M_PCPROTO if LLI provider */
typedef struct {
	ulong	dl_primitive;
	ulong	dl_dest_addr_length;
	ulong	dl_dest_addr_offset;
	ulong	dl_reserved;
	ulong	dl_errno;
} dl_uderror_ind_t;

/*
 * 	The following is a union of all the primitives.
 */
union DL_primitives {
	ulong			dl_primitive;
	dl_info_req_t		info_req;
	dl_info_ack_t		info_ack;
	dl_attach_req_t		attach_req;
	dl_detach_req_t		detach_req;
	dl_bind_req_t		bind_req;
	dl_bind_ack_t		bind_ack;
	dl_unbind_req_t		unbind_req;
	dl_ok_ack_t		ok_ack;
	dl_error_ack_t		error_ack;
	dl_unitdata_req_t	unitdata_req;
	dl_unitdata_ind_t	unitdata_ind;
	dl_uderror_ind_t	uderror_ind;
};


/*
 *	Structure/union size constants.
 */
#define DL_INFO_REQ_SIZE	sizeof(dl_info_req_t)
#define DL_INFO_ACK_SIZE	sizeof(dl_info_ack_t)
#define DL_ATTACH_REQ_SIZE	sizeof(dl_attach_req_t)
#define DL_DETACH_REQ_SIZE	sizeof(dl_detach_req_t)
#define DL_BIND_REQ_SIZE	sizeof(dl_bind_req_t)
#define DL_BIND_ACK_SIZE	sizeof(dl_bind_ack_t)
#define DL_UNBIND_REQ_SIZE	sizeof(dl_unbind_req_t)
#define DL_OK_ACK_SIZE		sizeof(dl_ok_ack_t)
#define DL_ERROR_ACK_SIZE	sizeof(dl_error_ack_t)
#define DL_UNITDATA_REQ_SIZE	sizeof(dl_unitdata_req_t)
#define DL_UNITDATA_IND_SIZE	sizeof(dl_unitdata_ind_t)
#define DL_UDERROR_IND_SIZE	sizeof(dl_uderror_ind_t)
#define DL_PRIMITIVES_SIZE	sizeof(union DL_primitives)

#endif
