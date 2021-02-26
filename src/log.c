/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   log.c
 * \brief   Logging
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
#include <stddef.h>

#include "config.h"
#include "debug.h"
#include "settings.h"

#include "log.h"


/** \brief  Log level labels
 *
 * The 'none' label should never be used, but is included for sanity checks.
 */
static const char * level_labels[] = {
    "[none   ]",
    "[debug  ]",
    "[info   ]",
    "[warning]",
    "[error  ]" };



static FILE *log_fp = NULL;
static int log_level;


void log_init(void)
{
    const char *path = NULL;

    settings_get_str("Monitor", "logfile", &path);
    settings_get_int("Monitor", "loglevel", &log_level);

    if (path != NULL) {
        log_fp = fopen(path, "wb");
        if (log_fp == NULL) {
            fprintf(stderr, "failed to open log file '%s',"
                    " reverting to stdout.\n",
                     path);
        }
    }
}


void log_exit(void)
{
    if (log_fp != NULL) {
        fclose(log_fp);
    }
}



void log_msg(log_level_t level, const char *msg, ...)
{
    va_list ap;

    if (log_level == 0
            || level >= sizeof level_labels / sizeof level_labels[1]) {
        return;
    }
    fprintf(log_fp, "[%s] ", level_labels[level]);
    vfprintf(log_fp, msg, ap);
    printf("\n");
    fflush(stdout);
}
