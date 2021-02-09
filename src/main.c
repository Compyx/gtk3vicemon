/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   main.c
 * \brief   Main program driver
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

#include "app-resources.h"
#include "appwindow.h"


/** \brief  Handler for the 'activate' event of the application
 *
 * \param[in]   app     Main application
 * \param[in]   data    extra event data (unused)
 */
static void on_app_activate(GtkApplication *app,
                            gpointer        data)
{
    GtkBuilder *builder;
    GMenuModel *app_menu;
    GtkWidget *window;

    builder = gtk_builder_new_from_resource(
            "/org/vice/gtk3vicemon/app-menu.xml");
    app_menu = G_MENU_MODEL(gtk_builder_get_object(builder, "app-menu"));
    gtk_application_set_menubar(app, app_menu);
    g_object_unref(builder);

    window = appwindow_create(app);
    gtk_widget_show_all(window);
}


/** \brief  Program entry point
 *
 * Sets up the Gtk application and handles any command line arguments.
 *
 * \param[in]   argc    argument count
 * \param[in]   argv    argument vector
 *
 * \return  non-0 on error
 */
int main(int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    app = gtk_application_new(
            "org.vice.gtk3vicemon",
            G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);

    app_register_resource();

    status = g_application_run(G_APPLICATION(app), argc, argv);

    app_unregister_resource();
    return status;
}
