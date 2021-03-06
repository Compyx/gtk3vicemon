/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   connection.c
 * \brief   Connection handler for to the binary monitor
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


#include "config.h"

#include "debug.h"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdbool.h>
#include "settings.h"
#include "monitor.h"

#include "vicemonapi.h"

#include "connection.h"


/** \brief  Connection FD
 */
static int connection_fd = 0;




/** \brief  Connection to the binary monitor interface
 *
 * Uses the settings 'VICE/host' (str) and 'VICE/port' (int).
 *
 * \return  TRUE on success
 */
bool connection_open(void)
{
    struct sockaddr_in sa;
    int result;
    const char *host = NULL;
    int port = 6502;


    /* get host and port from settings */
    debug_msg("Getting host from settings ('VICE/host'):");
    if (settings_get_str("VICE", "host", &host)) {
        debug_msg("OK, got '%s'.", host);
    } else {
        debug_msg("Couldn't find key, defaulting to '127.0.0.1'.");
        host = "127.0.0.1";
    }
    debug_msg("Getting port from settings ('VICE/port):");
    if (settings_get_int("VICE", "port", &port)) {
        debug_msg("OK, got %d.", port);
    } else {
        debug_msg("Couldn't find key, defaulting to 6502.");
        port = 6502;
    }


    sa.sin_family = AF_INET;
    sa.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, host, &sa.sin_addr);
    connection_fd = -1;

    debug_msg("Trying to connect to %s:%d.", host, port);

    connection_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connection_fd < 0) {
        debug_msg("Failed to open socket.");
        return false;
    }

    result = connect(connection_fd, (struct sockaddr *)&sa, sizeof(sa));
    if (result < 0) {
        debug_msg("failed to connect.");
        close(connection_fd);
        return false;
    }
    debug_msg("OK, connected.");
    return true;
}


void connection_close(void)
{
    if (connection_fd >= 0) {
        close(connection_fd);
        connection_fd = -1;
    }
}






/** \brief  Send command
 *
 * \param[in]   cmd command string
 * \param[in]   len length of \a command
 *
 * \return  request ID or -1 on error
 */
bool connection_send_cmd(const uint8_t *cmd, size_t len, uint32_t *id)
{
    uint8_t header[2] = { MON_STX, MON_API };

    send(connection_fd, header, 2,0);
    ssize_t result = send(connection_fd, cmd, len, 0);
    return result >= 0 ? true : false;
}



void connection_send_reset(void)
{
    const uint8_t reset_command[] = {
        0x01, 0x00, 0x00, 0x00,
        0xaf, 0xe9, 0x23, 0x3d,

        0xcc,

        0x00,
    };
    uint32_t req_id;

    connection_send_cmd(reset_command, sizeof(reset_command), &req_id);

}


mon_cmd_t *create_command(uint8_t type, const uint8_t *data, size_t len)
{
    mon_cmd_t *cmd = malloc(sizeof *cmd + len);

    cmd->cmd_len[0] = (len << 24) & 0xff;
    cmd->cmd_len[1] = (len << 16) & 0xff;
    cmd->cmd_len[2] = (len << 8) & 0xff;
    cmd->cmd_len[3] = len & 0xff;

    cmd->req_id[0] = 0;
    cmd->req_id[1] = 0;
    cmd->req_id[2] = 0;
    cmd->req_id[3] = 0;

    cmd->cmd_type = type;

    memcpy(cmd->cmd_body, data, len);

    return cmd;
}


void free_command(mon_cmd_t *cmd)
{
    free(cmd);
}
