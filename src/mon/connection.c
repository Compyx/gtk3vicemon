/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   connect_test.c
 * \brief   Test connecting to the binary monitor
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
#include <stdbool.h>

#define PORT 6510

static int fd;



bool connect_test(void)
{
    struct sockaddr_in sa;


    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr);

    int result;

    debug_msg("Trying to connect to %s:%d.", "127.0.0.1", PORT);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        debug_msg("Failed to open socket.");
        return false;
    }

    result = connect(fd, &sa, sizeof(sa));
    if (result < 0) {
        debug_msg("failed to connect.");
        close(fd);
        return false;
    }
    debug_msg("OK, connected.");
    return true;
}

#if 0
void connect_send_reset(void)
{
    const unsigned char reset_command[] = {
        0x02, 0x01, 
        0xff, 0xff, 0xff, 0xff, 
        0xaf, 0xe9, 0x23, 0x3d, 

        0xcc,

        0x00,
    };

}
#endif



