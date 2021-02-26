/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   settings.h
 * \brief   Settings management - header
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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <gtk/gtk.h>


#define SETTINGS_DIR    "~/.config/gtk3vicemon"

char *      settings_get_dir(void);
gboolean    settings_create_dir(void);

gboolean    settings_read(void);
gboolean    settings_write(void);

gboolean    settings_get_str(const char *group, const char *key, const char **value);
gboolean    settings_set_str(const char *group, const char *key, const char *value);
gboolean    settings_get_int(const char *group, const char *key, int *value);

gboolean    settings_init(void);
void        settings_exit(void);

#endif
