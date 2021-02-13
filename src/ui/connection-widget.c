/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   connection-widget.c
 * \brief   Statusbar connection widget
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
#include "debug.h"

#include "connection-widget.h"



static int connect_state = 0;



static gboolean on_led_draw(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    gint width;
    gint height;
    GtkStyleContext *context;

    context = gtk_widget_get_style_context(widget);
    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    /* clear widget I guess */
    gtk_render_background(context, cr, 0, 0, width, height);


   if (connect_state == 0) {
        /* not connected */
        cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
    } else {
        cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
    }
    /* render rectangle */
    cairo_rectangle(cr, 4.0, 10.0, 16.0, 8.0);
    cairo_fill(cr);
#if 0
    gtk_style_context_get_color(
            context,
            gtk_style_context_get_state (context),
            &color);
    gdk_cairo_set_source_rgba (cr, &color);
#endif
    cairo_fill (cr);

    return FALSE;
}


static GtkWidget *led_create(int state)
{
    GtkWidget *draw;


    debug_msg("Connent state = %s", state ? "true" : "fqlse");
    connect_state = state;

    draw = gtk_drawing_area_new();
    gtk_widget_set_size_request(draw, 32, 32);
    g_signal_connect(draw, "draw", G_CALLBACK(on_led_draw), NULL);
    return draw;
}



GtkWidget *connection_widget_create(int state)
{
    GtkWidget *grid;
    GtkWidget *led;
    GtkWidget *label;

    const char *host = NULL;
    const char *port = NULL;

    gchar buffer[1024];

    settings_get("VICE", "host", &host);
    settings_get("VICE", "port", &port);

    grid = gtk_grid_new();

    g_snprintf(buffer, sizeof(buffer), "%s:%s", host, port);
    label = gtk_label_new(buffer);

    connect_state = state;

    led = led_create(state);

    gtk_grid_attach(GTK_GRID(grid), led, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label, 1 ,0 , 1, 1);
    return grid;
}

