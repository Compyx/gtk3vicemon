/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   main.c
 * \brief   Main program driver
 *
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

#include "config.h"

#include <gtk/gtk.h>

#include "app-resources.h"
#include "appwindow.h"


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
