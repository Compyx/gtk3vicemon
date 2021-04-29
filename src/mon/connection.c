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

#include <glib.h>
#include <gio/gio.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdbool.h>
#include "settings.h"
#include "monitor.h"

#include "log.h"
#include "../ui/logview.h"
#include "vicemonapi.h"
#include "hexdump.h"

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

    logview_add(NULL, "Connecting to %s:%d: ", host, port);


    sa.sin_family = AF_INET;
    sa.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, host, &sa.sin_addr);
    connection_fd = -1;

    log_msg(LOG_INFO, "Connecting to %s:%d\n", host, port);

    debug_msg("Trying to connect to %s:%d.", host, port);

    connection_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connection_fd < 0) {
        debug_msg("Failed to open socket.");
        log_msg(LOG_ERR, "Failed open socket.\n");
        logview_add("err", "Failed to open socket.\n");
        return false;
    }

    result = connect(connection_fd, (struct sockaddr *)&sa, sizeof(sa));
    if (result < 0) {
        debug_msg("failed to connect.");
        log_msg(LOG_ERR, "Failed to connect to %s:%d\n", host, port);
        logview_add("err", "Failed to connect.\n");
        close(connection_fd);
        return false;
    }
    debug_msg("OK, connected.");
    log_msg(LOG_INFO, "OK\n");
    logview_add("ok", "OK.\n");
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


void connection_send_clearscreen(void)
{
    const uint8_t clear_cmd[] = {
        0x0f, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,

        0x72, 0x06,
        '\\', 'x', '9' ,'3',
        '\\', 'r'
    };
    uint32_t req_id;

    connection_send_cmd(clear_cmd, sizeof(clear_cmd), &req_id);
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
    if (data != NULL && len > 0) {
        memcpy(cmd->cmd_body, data, len);
    }

    return cmd;
}


ssize_t connection_get_response(void)
{
    mon_response_t response;
    ssize_t result;
    uint8_t buffer[256];

    result = recv(connection_fd, &buffer, sizeof(buffer), 0);
    if (result >= 12) {
        response.api_version = buffer[1];
        response.body_len[0] = buffer[2];
        response.body_len[1] = buffer[3];
        response.body_len[2] = buffer[4];
        response.body_len[3] = buffer[5];
        response.type = buffer[6];
        response.error_code = buffer[7];
        response.request_id[0] = buffer[8];
        response.request_id[1] = buffer[9];
        response.request_id[2] = buffer[10];
        response.request_id[3] = buffer[11];

        debug_msg("response API: %02x", response.api_version);
    }

    return result;
}



void free_command(mon_cmd_t *cmd)
{
    free(cmd);
}


static GSocketConnection *connection;
static GSocketClient *client;


/** \brief  Connect to the VICE binary monitor socket
 *
 * \return  bool
 */
gboolean connect_gio(void)
{
    const char *host = NULL;
    int port = 6502;
    GError *error = NULL;

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

    logview_add(NULL, "Connecting to %s:%d: ", host, port);


    client = g_socket_client_new();
    connection = g_socket_client_connect_to_host(
            client,
            host,
            (guint16)port,
            NULL,
            &error);
    if (error != NULL) {
        debug_msg("Error: %s\n", error->message);
        logview_add("err", "failed: %s\n", error->message);
        return FALSE;
    }
    logview_add("ok", "OK");
    return TRUE;
}


void connection_send_gio_reset(void)
{
    GOutputStream *ostream;
    GError *error = NULL;
    const uint8_t msg[] = {
        MON_STX, MON_API,
        1, 0, 0, 0, /* length = 1*/
        0, 1, 2, 3, /* req-id */
        0xcc,
        0x00
    };


    ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));
    g_output_stream_write(
            ostream,
            msg,
            sizeof(msg),
            NULL,
            &error);
    if (error != NULL) {
        debug_msg("error: %s\n", error->message);
    }
}


int connection_wait_for_response_type(int type, uint8_t *buffer, size_t *len)
{
    return 0;
}



uint32_t connection_get_vice_version(void)
{
    GOutputStream *ostream;
    GInputStream *istream;
    GError *error = NULL;
    const uint8_t msg[] = {
        MON_STX, MON_API,
        0, 0, 0, 0, /* length = 00 */
        0, 1, 2, 3, /* req-id */
        0x85,

    };
    uint8_t response[256] = { 0 };


    ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));

    g_output_stream_write(
            ostream,
            msg,
            sizeof(msg),
            NULL,
            &error);
    if (error != NULL) {
        debug_msg("error: %s\n", error->message);
    }

    if (error != NULL) {
        g_error_free(error);
        error = NULL;
    }


    istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    g_input_stream_read(
            istream,
            response,
            sizeof(response),
            NULL,
            &error);
    if (error != NULL) {
        debug_msg("Error: %s\n", error->message);
        g_error_free(error);
    }

    hexdump(response, sizeof(response));

    uint8_t main_ver_len = response[12];
    debug_msg("main version len = %d\n", main_ver_len);

    debug_msg("version = %d.%d.%d.%d\n",
            response[12] + '0',
            response[13] + '0',
            response[14] + '0',
            response[15] + '0');

    return 0;




}





void connection_close_gio(void)
{
}
