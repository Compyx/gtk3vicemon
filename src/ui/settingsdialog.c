/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   settingsdialog.c
 * \brief   Settings dialog
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
#include "debug.h"
#include "settings.h"

#include "settingsdialog.h"


/** \brief  Host name
 *
 * A GtkEntry to set the host name/IP
 */
static GtkWidget *host_widget;


/** \brief  Port number
 *
 * A GtkSpinButton to set the host port (0-65535)
 */
static GtkWidget *port_widget;



/** \brief  Handler for the 'response' event of the settings dialog
 *
 * \param[in]   dialog      dialog
 * \param[in]   response_id response ID
 * \param[in]   data        extra event data
 */
static void on_response(GtkDialog *dialog, int response_id, gpointer data)
{
    switch (response_id) {
        case GTK_RESPONSE_ACCEPT:
            debug_msg("TODO: Save settings.");
            gtk_widget_destroy(GTK_WIDGET(dialog));
            break;
        case GTK_RESPONSE_REJECT:
            gtk_widget_destroy(GTK_WIDGET(dialog));
            break;
        default:
            break;
    }
}


/** \brief  Create a left-aligned, 16 pixels left-indented label with \a text
 *
 * Uses gtk_label_set_markup(), so Pango markup is supported.
 *
 * \param[in]   text    label text
 */
static GtkWidget *create_indented_label(const char *text)
{
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), text);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    g_object_set(label, "margin-left", 16, NULL);
    return label;
}


/** \brief  Create widgets for the 'VICE' section of the ini file
 *
 * \return  GtkGrid
 */
static GtkWidget *create_vice_section(void)
{
    GtkWidget *grid;
    GtkWidget *label;
    const char *host;
    int port;

    settings_get_str("VICE", "host", &host);
    settings_get_int("VICE", "port", &port);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<b>VICE</b>");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

    label = create_indented_label("Host:");
    host_widget = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(host_widget), host);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), host_widget, 1, 1, 1, 1);

    label = create_indented_label("Port:");
    port_widget = gtk_spin_button_new_with_range(0.0, 65535.0, 1.0);
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(port_widget), 0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(port_widget), (gdouble)port);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), port_widget, 1, 2, 1, 1);

    return grid;
}



/** \brief  Create content widget for the dialog
 *
 *
 * \return GtkGrid
 */
static GtkWidget *create_content_widget(void)
{
    GtkWidget *grid;

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    g_object_set(grid, "margin-bottom", 16, NULL);

    gtk_grid_attach(GTK_GRID(grid), create_vice_section(), 0, 0, 1, 1);

    return grid;
}


/** \brief  Create settings dialog
 *
 * \param[in]   parent  parent widget
 *
 * \return  GtkDialog
 */
GtkWidget *settings_dialog_new(GtkWidget *parent)
{
    GtkWidget *dialog;
    GtkWidget *content;

    dialog = gtk_dialog_new_with_buttons(
            "Settings",
            GTK_WINDOW(parent),
            GTK_DIALOG_MODAL,
            "Save",
            GTK_RESPONSE_ACCEPT,
            "Cancel",
            GTK_RESPONSE_REJECT,
            NULL);

    content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content), create_content_widget());

    g_signal_connect(dialog, "response", G_CALLBACK(on_response), NULL);

    gtk_widget_show_all(dialog);
    return dialog;
}
