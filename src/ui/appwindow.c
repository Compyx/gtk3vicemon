/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   appwindow.c
 * \brief   Main application window
 *
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

#include "config.h"

#include <gtk/gtk.h>


#include "appwindow.h"


GtkWidget *appwindow_create(GtkApplication *app)
{
  GtkWidget *window;

  window = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
  gtk_window_set_title(GTK_WINDOW(window), "Gtk3 VICE Monitor");
  return window;
}
