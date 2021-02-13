/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   statusbar.c
 * \brief   Main window statusbar
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

#include <gtk/gtk.h>

#include "settings.h"
#include "connection-widget.h"

#include "statusbar.h"


GtkWidget *statusbar_create(int state)
{
    GtkWidget *grid;
    GtkWidget *connection;

    grid = gtk_grid_new();
    gtk_widget_set_hexpand(grid, FALSE);
    gtk_widget_set_valign(grid, GTK_ALIGN_END);

    connection = connection_widget_create(state);
    gtk_grid_attach(GTK_GRID(grid), connection, 0, 0, 1, 1);

    gtk_widget_show_all(grid);
    return grid;
}
