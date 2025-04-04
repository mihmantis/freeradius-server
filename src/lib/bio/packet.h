#pragma once
/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/**
 * $Id$
 * @file lib/bio/packet.h
 * @brief Binary IO abstractions for #fr_radius_packet_t
 *
 * @copyright 2024 Network RADIUS SAS (legal@networkradius.com)
 */
RCSIDH(lib_bio_packet_h, "$Id$")

#include <freeradius-devel/util/packet.h>
#include <freeradius-devel/bio/base.h>

// @todo - _CONST

typedef struct fr_bio_packet_s fr_bio_packet_t;

/** Read a packet and pairs from the network
 *
 * @param bio		the packet-based bio
 * @param packet_ctx	any per-packet additional context needed
 * @param packet_p	the output packet descriptor.  Contains raw protocol data (IDs, counts, etc.)
 * @param out		the decoded pairs from the packet
 * @return
 *	- <0 on error
 *	- 0 for success (*packet_p may still be NULL tho)
 */
typedef int (*fr_bio_packet_read_t)(fr_bio_packet_t *bio, void *packet_ctx, fr_radius_packet_t **packet_p, fr_pair_list_t *out);

/** Write a packet and pairs from the network
 *
 * @param bio		the packet-based bio
 * @param packet_ctx	any per-packet additional context needed (e.g. request for reply)
 * @param packet	the output packet descriptor.  Contains raw protocol data (IDs, counts, etc.)
 * @param list		the pairs to encode in the packet
 * @return
 *	- <0 on error (EOF, fail, etc,)
 *	- 0 for success
 */
typedef int (*fr_bio_packet_write_t)(fr_bio_packet_t *bio, void *packet_ctx, fr_radius_packet_t *packet, fr_pair_list_t *list);

/** Release an outgoing packet.
 *
 * @param bio		the packet-based bio
 * @param packet_ctx	any per-packet additional context needed (e.g. request for reply)
 * @param packet	the output packet descriptor.  Contains raw protocol data (IDs, counts, etc.)
 * @return
 *	- <0 on error
 *	- 0 for success
 */
typedef int (*fr_bio_packet_release_t)(fr_bio_packet_t *bio, void *packet_ctx, fr_radius_packet_t *packet);

struct fr_bio_packet_s {
	void			*uctx;		//!< user ctx, caller can manually set it.

	fr_bio_packet_read_t	read;		//!< read from the underlying bio
	fr_bio_packet_write_t	write;		//!< write to the underlying bio

	fr_bio_packet_release_t	release;		//!< cancel an outstanding packet

	fr_bio_t		*bio;		//!< underlying bio for IO
};


static inline CC_HINT(nonnull) int fr_bio_packet_read(fr_bio_packet_t *bio, void *packet_ctx, fr_radius_packet_t **packet_p, fr_pair_list_t *out)
{
	return bio->read(bio, packet_ctx, packet_p, out);
}

static inline CC_HINT(nonnull) int fr_bio_packet_write(fr_bio_packet_t *bio, void *packet_ctx, fr_radius_packet_t *packet, fr_pair_list_t *list)
{
	return bio->write(bio, packet_ctx, packet, list);
}

static inline CC_HINT(nonnull) int fr_bio_packet_release(fr_bio_packet_t *bio, void *packet_ctx, fr_radius_packet_t *packet)
{
	if (!bio->release) return 0;

	return bio->release(bio, packet_ctx, packet);
}
