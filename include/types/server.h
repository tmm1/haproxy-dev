/*
  include/types/server.h
  This file defines everything related to servers.

  Copyright (C) 2000-2006 Willy Tarreau - w@1wt.eu
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, version 2.1
  exclusively.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _TYPES_SERVER_H
#define _TYPES_SERVER_H

#include <netinet/in.h>
#include <arpa/inet.h>

#include <haproxy/mini-clist.h>

#include <types/buffers.h>
#include <types/proxy.h>
#include <types/queue.h>
#include <types/task.h>


/* server flags */
#define SRV_RUNNING	1	/* the server is UP */
#define SRV_BACKUP	2	/* this server is a backup server */
#define	SRV_MAPPORTS	4	/* this server uses mapped ports */
#define	SRV_BIND_SRC	8	/* this server uses a specific source address */
#define	SRV_CHECKED	16	/* this server needs to be checked */

/* function which act on servers need to return various errors */
#define SRV_STATUS_OK       0   /* everything is OK. */
#define SRV_STATUS_INTERNAL 1   /* other unrecoverable errors. */
#define SRV_STATUS_NOSRV    2   /* no server is available */
#define SRV_STATUS_FULL     3   /* the/all server(s) are saturated */
#define SRV_STATUS_QUEUED   4   /* the/all server(s) are saturated but the connection was queued */


struct server {
	struct server *next;
	int state;				/* server state (SRV_*) */
	int  cklen;				/* the len of the cookie, to speed up checks */
	char *cookie;			/* the id set in the cookie */
	char *id;				/* just for identification */
	struct list pendconns;		/* pending connections */
	int nbpend, nbpend_max;		/* number of pending connections */
	struct task *queue_mgt;		/* the task associated to the queue processing */
	struct sockaddr_in addr;		/* the address to connect to */
	struct sockaddr_in source_addr;	/* the address to which we want to bind for connect() */
	short check_port;			/* the port to use for the health checks */
	int health;				/* 0->rise-1 = bad; rise->rise+fall-1 = good */
	int rise, fall;			/* time in iterations */
	int inter;				/* time in milliseconds */
	int result;				/* 0 = connect OK, -1 = connect KO */
	int curfd;				/* file desc used for current test, or -1 if not in test */
	unsigned char uweight, eweight;	/* user-specified weight-1, and effective weight-1 */
	unsigned int wscore;		/* weight score, used during srv map computation */
	int cur_sess, cur_sess_max;		/* number of currently active sessions (including syn_sent) */
	unsigned int cum_sess;		/* cumulated number of sessions really sent to this server */
	unsigned int maxconn, minconn;	/* max # of active sessions (0 = unlimited), min# for dynamic limit. */
	unsigned failed_checks, down_trans;	/* failed checks and up-down transitions */
	unsigned failed_conns, failed_resp;	/* failed connect() and responses */
	unsigned failed_secu;		/* blocked responses because of security concerns */
	struct proxy *proxy;		/* the proxy this server belongs to */
};


#endif /* _TYPES_SERVER_H */

/*
 * Local variables:
 *  c-indent-level: 8
 *  c-basic-offset: 8
 * End:
 */