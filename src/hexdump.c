/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   hexdump.c
 * \brief   Hexdump data
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hexdump.h"


#define COLUMNS 16

void hexdump(const uint8_t *data, size_t len)
{
    uint8_t display[COLUMNS];

    memset(display, 0, COLUMNS);

    for (size_t row = 0; row * COLUMNS < len; row++) {
        printf("%04x: ", (unsigned int)(row * COLUMNS));
        for (size_t col = 0; col < COLUMNS && (row * COLUMNS) + col < len; col++) {
            display[col] = data[col + row * COLUMNS];
            printf("%02x ", data[col + row * COLUMNS]);
        }
        printf("\n");
    }
}
