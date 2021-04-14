/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   logview.c
 * \brief   Log message view
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
#include <glib/gprintf.h>
#include <stdbool.h>
#include <stdint.h>

#include "debug.h"
#include "connection.h"
#include "log.h"
#include "logview.h"


static GtkWidget *log_textview;


static void create_tags(void)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_textview));


    gtk_text_buffer_create_tag(
            buffer,
            "ok",
            "foreground", "green",
            NULL);

    gtk_text_buffer_create_tag(
            buffer,
            "err",
            "foreground", "red",
            NULL);
}




GtkWidget *logview_create(void)
{
    GtkWidget *grid;
    GtkWidget *scrolled;


    grid = gtk_grid_new();

    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(scrolled, TRUE);
    log_textview = gtk_text_view_new();
    create_tags();
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(log_textview), TRUE);
    gtk_widget_set_hexpand(log_textview, TRUE);
    gtk_container_add(GTK_CONTAINER(scrolled), log_textview);
    gtk_grid_attach(GTK_GRID(grid), scrolled, 0, 0, 1, 1);

    return grid;
}


void logview_add(const char *tag, const char *msg, ...)
{
    GtkTextBuffer *buffer;
    gchar *temp = NULL;
    va_list ap;
    GtkTextIter start;
    GtkTextIter end;

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_textview));
    va_start(ap, msg);
    g_vasprintf(&temp, msg, ap);
    va_end(ap);


    gtk_text_buffer_get_bounds(buffer, &start, &end);

    gtk_text_buffer_insert_with_tags_by_name(
            buffer,
            &end,
            temp,
            -1,
            tag,
            NULL);
    g_free(temp);
}
