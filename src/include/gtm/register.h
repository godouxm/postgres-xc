/*-------------------------------------------------------------------------
 *
 * register.h
 *
 *
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2010-2011 Nippon Telegraph and Telephone Corporation
 *
 * src/include/gtm/register.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef _REGISTER_H
#define _REGISTER_H

#include "gtm/gtm_c.h"
#include "gtm/gtm_lock.h"
#include "gtm/gtm_list.h"
#include "gtm/stringinfo.h"

/*
 * This structure represents the data that is saved each time a Postgres-XC node
 * registered on GTM.
 * It contains:
 *	-	Type of the Node: Proxy, Coordinator, Datanode
 *	-	Node number
 *	-	Proxy number: This ID number is set at 0 if node does not go through a Proxy
 *		or if Node Type is Proxy
 *	-	PostgreSQL port the node uses to communicate
 *	-	IP visible to GTM
 *	-	Data folder of the node
 */

typedef enum GTM_PGXCNodeStatus
{
	NODE_CONNECTED,
	NODE_DISCONNECTED
} GTM_PGXCNodeStatus;

typedef struct GTM_PGXCNodeInfo
{
	GTM_PGXCNodeType	type;		/* Type of node */
	GTM_PGXCNodeId		nodenum;	/* Node number */
	GTM_PGXCNodeId		proxynum;	/* Proxy number the node goes through */
	GTM_PGXCNodePort	port;		/* Port number of the node */
	char			   *ipaddress;	/* IP address of the nodes */
	char			   *datafolder;	/* Data folder of the node */
	GTM_PGXCNodeStatus	status;		/* Node status */
	GTM_RWLock			node_lock;	/* Lock on this structure */
	int					socket;		/* socket number used for registration */
} GTM_PGXCNodeInfo;

int Recovery_PGXCNodeRegister(GTM_PGXCNodeType	type,
							  GTM_PGXCNodeId	nodenum,
							  GTM_PGXCNodePort	port,
							  GTM_PGXCNodeId	proxynum,
							  GTM_PGXCNodeStatus status,
							  char			   *ipaddress,
							  char			   *datafolder,
							  bool				in_recovery,
							  int				socket);
int Recovery_PGXCNodeUnregister(GTM_PGXCNodeType type,
								GTM_PGXCNodeId nodenum,
								bool in_recovery,
								int socket);
int Recovery_PGXCNodeBackendDisconnect(GTM_PGXCNodeType type, GTM_PGXCNodeId nodenum, int socket);

void Recovery_RecordRegisterInfo(GTM_PGXCNodeInfo *nodeinfo, bool is_register);
void Recovery_RestoreRegisterInfo(void);
void Recovery_SaveRegisterInfo(void);
void Recovery_PGXCNodeDisconnect(Port *myport);
void Recovery_SaveRegisterFileName(char *dir);

void ProcessPGXCNodeRegister(Port *myport, StringInfo message);
void ProcessPGXCNodeUnregister(Port *myport, StringInfo message);
void ProcessPGXCNodeBackendDisconnect(Port *myport, StringInfo message);

#endif /* GTM_NODE_H */
