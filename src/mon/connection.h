/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   connect_test.h
 * \brief   Test connecting to the binary monitor - header
 *
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

/*
    Gtk3 VICE Monitor
    Copyright (C) 2021  Bas Wassink

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    This General Public License does not permit incorporating your program into
    proprietary programs.  If your program is a subroutine library, you may
    consider it more useful to permit linking proprietary applications with the
    library.  If this is what you want to do, use the GNU Lesser General
    Public License instead of this License.
*/


#ifndef MON_CONNECTION_H_
#define MON_CONNECTION_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


/** \brief  Monitor command object
 */
typedef struct mon_cmd_s {
    uint8_t cmd_len[4];     /**< command length (4) */
    uint8_t req_id[4];      /**< request ID (4) */
    uint8_t cmd_type;       /**< command type */
    uint8_t cmd_body[];     /**< command body (variable) */
} mon_cmd_t;


bool connection_open(void);
void connection_close(void);

bool connection_send_cmd(const uint8_t *cmd, size_t len, uint32_t *req_id);
void connection_send_reset(void);
void connection_send_clearscreen(void);

mon_cmd_t *create_command(uint8_t type, const uint8_t *data, size_t len);
void free_command(mon_cmd_t *cmd);
#endif
