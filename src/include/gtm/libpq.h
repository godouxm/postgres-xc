/*-------------------------------------------------------------------------
 *
 * libpq.h
 *	  POSTGRES LIBPQ buffer structure definitions.
 *
 *
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2010-2011 Nippon Telegraph and Telephone Corporation
 *
 * src/include/gtm/libpq.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef LIBPQ_H
#define LIBPQ_H

#include <sys/types.h>
#include <netinet/in.h>

#include "gtm/stringinfo.h"
#include "gtm/libpq-be.h"

/*
 * External functions.
 */

/*
 * prototypes for functions in pqcomm.c
 */
extern int StreamServerPort(int family, char *hostName,
		 unsigned short portNumber, int ListenSocket[],
				 int MaxListen);
extern int	StreamConnection(int server_fd, Port *port);
extern void StreamClose(int sock);
extern void TouchSocketFile(void);
extern void pq_comm_reset(void);
extern int	pq_getbytes(Port *myport, char *s, size_t len);
extern int	pq_getstring(Port *myport, StringInfo s);
extern int	pq_getmessage(Port *myport, StringInfo s, int maxlen);
extern int	pq_getbyte(Port *myport);
extern int	pq_peekbyte(Port *myport);
extern int	pq_putbytes(Port *myport, const char *s, size_t len);
extern int	pq_flush(Port *myport);
extern int	pq_putmessage(Port *myport, char msgtype, const char *s, size_t len);

#endif   /* LIBPQ_H */
