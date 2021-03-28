/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) passthru.h: version 24.1 created on 10/28/91 at 17:53:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)passthru.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*
**  passthru driver include file
*/

/*
**  ioctl commands
*/
#define	PT_SET_CMD        ((('P' * 256) + 'S' * 256) + 'C')     
#define	PT_EXECUTE        (('P' * 256 )+ 'E' )
#define	PT_GET_RESULTS    ((('P' * 256) + 'G' * 256) + 'R')

	
/*
** This structure is passwd from the user program to the passthru driver, 
** via the ioctl system call.  It contains the SCSI command to be executed.
*/
struct scsi_cmd {
        ushort cmd_dsc_len;          /* The length in bytes of the scsi command 
                                        descriptor block */
        unchar  cmd_dsc_blk[14];     /* The scsi command descriptor block */
        uint    mil_sec;             /* The number of seconds to wait after the 
                                        command has been issued before returning*/
};


/*
**  The "scsi_result" structure contains return codes, including any error 
**  codes from the low level driver.
*/
struct scsi_result {
         ushort    err_code;      /* the error code from the low level driver*/
         ushort    err_len;       /* the size of error status buffer */	
         unchar    error[28];     /* the error status buffer */
         uint      act_time;      /* the actual time the command took to run */
         uint      disc_count;    /* the number of disconnets that happend while 
                                     running this command  */
};


#define SCSI_CMD struct scsi_cmd
#define SCSI_RESULT struct scsi_result

