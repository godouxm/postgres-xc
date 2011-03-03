/*-------------------------------------------------------------------------
 *
 * pqcomm.h
 *		Definitions common to frontends and backends.
 *
 * NOTE: for historical reasons, this does not correspond to pqcomm.c.
 * pqcomm.c's routines are declared in libpq.h.
 *
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2010-2011 Nippon Telegraph and Telephone Corporation
 *
 * src/include/gtm/pqcomm.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PQCOMM_H
#define PQCOMM_H

#include <sys/socket.h>
#include <netdb.h>
#ifdef HAVE_SYS_UN_H
#include <sys/un.h>
#endif
#include <netinet/in.h>

typedef struct
{
	struct sockaddr_storage addr;
	size_t	salen;
} SockAddr;

/* Configure the UNIX socket location for the well known port. */

#define UNIXSOCK_PATH(path, port, sockdir) \
		snprintf(path, sizeof(path), "%s/.s.PGSQL.%d", \
				((sockdir) && *(sockdir) != '\0') ? (sockdir) : \
				DEFAULT_PGSOCKET_DIR, \
				(port))

/*
 * Packet lengths are 4 bytes in network byte order.
 *
 * The initial length is omitted from the packet layouts appearing below.
 */

typedef uint32 PacketLen;

/*
 * In protocol 3.0 and later, the startup packet length is not fixed, but
 * we set an arbitrary limit on it anyway.	This is just to prevent simple
 * denial-of-service attacks via sending enough data to run the server
 * out of memory.
 */
#define MAX_STARTUP_PACKET_LENGTH 10000

#endif   /* PQCOMM_H */
