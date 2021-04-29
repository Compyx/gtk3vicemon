/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   appwindow.c
 * \brief   Main application window
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
#include <stdbool.h>

#include "debug.h"
#include "log.h"
#include "statusbar.h"
#include "connection.h"
#include "logview.h"

#include "appwindow.h"


/** \brief  Handler for the 'destroy 'even of the main application window
 *
 * Disconnects from the binary monitor.
 *
 * \param[in]   window  widget triggering the event
 * \param[in]   data    extra even data (unused)
 */
static void on_destroy(GtkWidget *window, gpointer data)
{
    debug_msg("Destroy caught, disconnecting from binary monitor.");
    log_msg(LOG_INFO, "Exiting application.\n");
    log_exit();
    connection_close();
}


/** \brief  Create the main application window
 *
 * Attempts to open a connection to the remove vice monitor.
 *
 * \param[in]   app GtkApplication
 *
 * \return  GtkApplicationWindow
 */
GtkWidget *appwindow_create(GtkApplication *app)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *logview;
    GtkWidget *statusbar;
    bool conn_res;

    window = gtk_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    gtk_window_set_title(GTK_WINDOW(window), "Gtk3 VICE Monitor");

    grid = gtk_grid_new();


    logview = logview_create();
    gtk_grid_attach(GTK_GRID(grid), logview, 0, 0, 1, 1);

    conn_res = connect_gio();
    statusbar = statusbar_create(conn_res);
    gtk_grid_attach(GTK_GRID(grid), statusbar, 0, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(window), grid);

    if (conn_res) {
        connection_send_gio_reset();
        connection_get_vice_version();
    }

    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    return window;
}
