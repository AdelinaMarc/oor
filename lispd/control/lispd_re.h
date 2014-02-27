/*
 * lispd_re_control.h
 *
 * This file is part of LISP Mobile Node Implementation.
 * Various routines to manage the list of interfaces.
 *
 * Copyright (C) 2011 Cisco Systems, Inc, 2011. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Please send any bug reports or fixes you make to the email address(es):
 *    LISP-MN developers <devel@lispmob.org>
 *
 * Written or modified by:
 *    Florin Coras   <fcoras@ac.upc.edu>
 *
 */

#ifndef LISPD_RE_CONTROL_H_
#define LISPD_RE_CONTROL_H_

#include "lispd_remdb.h"
#include <lispd_mapping.h>
#include <lisp_messages.h>

#define MCASTMIN4   0xE0000000
#define MCASTMAX4   0xEFFFFFFF

/*
 * Structure to expand the lispd_mapping_elt to support multicast info AFI
 */

typedef struct {
    locator_t       *locator;
    lisp_addr_t     *delivery_rloc;
//    nonces_list     *nonces;
//    timer           *probe_timer;
    int             join_pending;
    int             leave_pending;
} re_upstream_t;

typedef struct mcinfo_mapping_exteded_info_ {
    remdb_t         *jib;       /* joining information base - the joined downstreams */
    re_upstream_t   *upstream;  /* the overlay parent */
} mcinfo_mapping_extended_info;


int re_join_channel(lisp_addr_t *mceid);
int re_join_upstream(mapping_t *ch_mapping);
int re_leave_channel(lisp_addr_t *mceid);
int re_leave_upstream(mapping_t *ch_mapping);

int re_recv_join_request(lisp_addr_t *ch, lisp_addr_t *rloc_pair);


int re_recv_leave_request(lisp_addr_t *ch, lisp_addr_t *rloc_pair);
int re_send_leave_ack();

int re_recv_join_ack(lisp_addr_t *eid, uint32_t nonce);
int re_recv_leave_ack(lisp_addr_t *eid, uint32_t nonce);


re_upstream_t       *re_get_upstream(lisp_addr_t *eid);
remdb_t             *re_get_jib(lisp_addr_t *mcaddr);

glist_t    *re_get_orlist(lisp_addr_t *addr);

static inline remdb_t *mapping_get_jib(mapping_t *mapping) {
    return(((mcinfo_mapping_extended_info*)mapping->extended_info)->jib);
}

static inline re_upstream_t *mapping_get_re_upstream(mapping_t *mapping) {
    return(((mcinfo_mapping_extended_info*)mapping->extended_info)->upstream);
}

static inline void re_upstream_del(re_upstream_t *upstream) {
    free_locator(upstream->locator);
    lisp_addr_del(upstream->delivery_rloc);
    free(upstream);
}

#endif /* LISPD_RE_CONTROL_H_ */