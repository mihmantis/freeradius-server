#pragma once
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/**
 * $Id$
 *
 * @file protocols/radius/client.h
 * @brief RADIUS bio handlers for outgoing RADIUS client sockets
 *
 * @copyright 2024 Network RADIUS SAS (legal@networkradius.com)
 */
RCSIDH(radius_client_h, "$Id$")

#include <freeradius-devel/radius/radius.h>
#include <freeradius-devel/radius/bio.h>
#include <freeradius-devel/bio/packet.h>
#include <freeradius-devel/bio/fd.h>

typedef struct {
	fr_radius_bio_verify_t	verify;

	fr_retry_config_t  retry[FR_RADIUS_CODE_MAX];
} fr_radius_client_config_t;

fr_bio_packet_t *fr_radius_client_bio_alloc(TALLOC_CTX *ctx, fr_radius_client_config_t *cfg, fr_bio_fd_config_t const *fd_cfg) CC_HINT(nonnull);

fr_bio_t	*fr_radius_client_bio_get_fd(fr_bio_packet_t *bio);
