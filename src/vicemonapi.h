/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   vicemonapi.h
 * \brief   Binary monitor API
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

#ifndef VICEMONAPI_H_
#define VICEMONAPI_H_


/** \brief  Binary monitor commands
 */
typedef enum vicemon_command_e {
    MON_CMD_INVALID = 0x00,

    MON_CMD_MEM_GET = 0x01,
    MON_CMD_MEM_SET = 0x02,

    MON_CMD_CHECKPOINT_GET = 0x11,
    MON_CMD_CHECKPOINT_SET = 0x12,
    MON_CMD_CHECKPOINT_DELETE = 0x13,
    MON_CMD_CHECKPOINT_LIST = 0x14,
    MON_CMD_CHECKPOINT_TOGGLE = 0x15,

    MON_CMD_CONDITION_SET = 0x22,

    MON_CMD_REGISTERS_GET = 0x31,
    MON_CMD_REGISTERS_SET = 0x32,

    MON_CMD_DUMP = 0x41,
    MON_CMD_UNDUMP = 0x42,

    MON_CMD_RESOURCE_GET = 0x51,
    MON_CMD_RESOURCE_SET = 0x52,

    MON_CMD_ADVANCE_INSTRUCTIONS = 0x71,
    MON_CMD_KEYBOARD_FEED = 0x72,
    MON_CMD_EXECUTE_UNTIL_RETURN = 0x73,

    MON_CMD_PING = 0x81,
    MON_CMD_BANKS_AVAILABLE = 0x82,
    MON_CMD_REGISTERS_AVAILABLE = 0x83,
    MON_CMD_DISPLAY_GET = 0x84,

    MON_CMD_EXIT = 0xaa,
    MON_CMD_QUIT = 0xbb,
    MON_CMD_RESET = 0xcc,
    MON_CMD_AUTOSTART = 0xdd,
} vicemon_command_t;


typedef enum vicemon_response {
    MON_RESPONSE_MEM_GET = 0x01,
    MON_RESPONSE_MEM_SET = 0x02,

    MON_RESPONSE_CHECKPOINT_INFO = 0x11,

    MON_RESPONSE_CHECKPOINT_DELETE = 0x13,
    MON_RESPONSE_CHECKPOINT_LIST = 0x14,
    MON_RESPONSE_CHECKPOINT_TOGGLE = 0x15,

    MON_RESPONSE_CONDITION_SET = 0x22,

    MON_RESPONSE_REGISTER_INFO = 0x31,

    MON_RESPONSE_DUMP = 0x41,
    MON_RESPONSE_UNDUMP = 0x42,

    MON_RESPONSE_RESOURCE_GET = 0x51,
    MON_RESPONSE_RESOURCE_SET = 0x52,

    MON_RESPONSE_JAM = 0x61,
    MON_RESPONSE_STOPPED = 0x62,
    MON_RESPONSE_RESUMED = 0x63,

    MON_RESPONSE_ADVANCE_INSTRUCTIONS = 0x71,
    MON_RESPONSE_KEYBOARD_FEED = 0x72,
    MON_RESPONSE_EXECUTE_UNTIL_RETURN = 0x73,

    MON_RESPONSE_PING = 0x81,
    MON_RESPONSE_BANKS_AVAILABLE = 0x82,
    MON_RESPONSE_REGISTERS_AVAILABLE = 0x83,
    MON_RESPONSE_DISPLAY_GET = 0x84,

    MON_RESPONSE_EXIT = 0xaa,
    MON_RESPONSE_QUIT = 0xbb,
    MON_RESPONSE_RESET = 0xcc,
    MON_RESPONSE_AUTOSTART = 0xdd,
} vicemon_responst;


typedef enum vicemon_error_e {
    MON_ERR_OK = 0x00,
    MON_ERR_OBJECT_MISSING = 0x01,
    MON_ERR_INVALID_MEMSPACE = 0x02,
    MON_ERR_CMD_INVALID_LENGTH = 0x80,
    MON_ERR_INVALID_PARAMETER = 0x81,
    MON_ERR_CMD_INVALID_API_VERSION = 0x82,
    MON_ERR_CMD_INVALID_TYPE = 0x83,
    MON_ERR_CMD_FAILURE = 0x8f,
} vicemon_error_t;

#endif

