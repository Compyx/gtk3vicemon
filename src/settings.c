/* vim: set et ts=4 sw=4 sts=4 syntax=c.doxygen: */

/** \file   settings.c
 * \brief   Settings management
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
#include <glib.h>
#include <glib/gstdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "debug.h"
#include "app-resources.h"


#include "settings.h"


/** \brief  Reference to the settings.ini file
 */
static GKeyFile *keyfile;


/** \brief  Get XDG path to the application config dir
 *
 * \return  config dir, free with g_free()
 */
char *settings_get_dir(void)
{
    char *tmp;

    tmp = getenv("XDG_CONFIG_HOME");
    if (tmp == NULL) {
        return g_strconcat(getenv("HOME"), "/.config/gtk3vicemon", NULL);
    } else {
        return g_strconcat(tmp, "/gtk3vicemon", NULL);
    }
}



/** \brief  Create XDG path to the application config dir
 *
 * This function does a couple of things.
 *
 * First it checks the XDG_CONFIG_HOME environment variable to find the user's
 * config dir, if that fails the config dir is set to the XDG default of
 * `$HOME/.config`.
 * It then checks if `HOME/.config` exists and if it doesn't, it tries to create
 * the directory. If that fails, the function will return FALSE and the application
 * should exit.
 *
 * The it checks if $XDG_HOME/gtk3vicemon exists and is a directory. If it isn't
 * found the directory is created and if it is found but not a directory this
 * function will return FALSE.
 *
 * \return  TRUE on success
 */
gboolean settings_create_dir(void)
{
    GStatBuf st;
    char *tmp;
    char *config_home;
    char *config_path;

    /* get XDG_CONFIG_HOME, otherwise create the string */
    debug_msg("Checking $XDG_CONFIG_HOME");
    tmp = getenv("XDG_CONFIG_HOME");
    if (tmp == NULL) {
        char *home;

        debug_msg("Null, construct $HOME/.config");
        home = getenv("HOME");
        config_home = g_strconcat(home, "/.config", NULL);
    } else {
        config_home = g_strdup(tmp);
    }

    /* check if XDG_CONFIG_HOME exists, otherwise try to create it */
    debug_msg("Getting info on '%s':", config_home);
    if (g_stat(config_home, &st) < 0) {
        debug_msg("Not found, try to create:");
        if (g_mkdir(config_home, 0755) < 0) {
            debug_msg("couldn't create dir, giving up.");
            g_free(config_home);
            return FALSE;
        }
    } else {
        if ((st.st_mode & S_IFDIR) == 0) {
            debug_msg("'%s' is not a directory, giving up.", config_home);
            g_free(config_home);
            return FALSE;
        } else {
            debug_msg("OK, '%s' is a directory.", config_home);
        }
    }

    /* check for ~/.config/gtk3vicemon */
    config_path = g_strconcat(config_home, "/gtk3vicemon", NULL);
    g_free(config_home);
    debug_msg("Checking for '%s'.", config_path);
    if (g_stat(config_path, &st) < 0) {
        debug_msg("Doesn't exist, create.");
        if (g_mkdir(config_path, 0755) < 0) {
            debug_msg("Failed to create directory, giving up.");
            g_free(config_path);
            return FALSE;
        }
        debug_msg("OK");
    }
    /* path exists, is it a directory? */
    if ((st.st_mode & S_IFDIR) == 0) {
        debug_msg("Exists, but is not a directory, giving up.");
        g_free(config_path);
        return FALSE;
    }
    debug_msg("OK.");
    g_free(config_path);
    return TRUE;
}


/** \brief  Read the settings from file and if that fails create the setting in memory.
 *
 * Attempts to read `~/.config/gtk3vicemon/settings.ini`.
 *
 * If that fails it will read the default configuration from the app resources.
 *
 * \return  FALSE if both reading the settings from file and reading from the
 *          resources failed
 */
gboolean settings_read(void)
{
    char *path;
    char *config;
    GError *err = NULL;

    config = settings_get_dir();
    path = g_strconcat(config, "/settings.ini", NULL);

    debug_msg("Attempting to load '%s'.", path);
    /* attempt to load setting file */
    if (!g_key_file_load_from_file(
                keyfile,
                path,
                G_KEY_FILE_KEEP_COMMENTS,
                &err)) {

        GError *err2 = NULL;
        GError *err3 = NULL;

        debug_msg("error %d: %s", err->code, err->message);
        /* use default settings */
        debug_msg("Attempting to load default settings from GResource.");
        GBytes *data = g_resources_lookup_data(
                "/org/vice/gtk3vicemon/settings.ini",
                G_RESOURCE_FLAGS_NONE,
                &err2);
        if (data == NULL) {
            error_msg("Data is null: %d: %s", err2->code, err2->message);
            return FALSE;
        }

        /* Store default settings in the keyfile */
        if (!g_key_file_load_from_bytes(
                keyfile,
                data,
                0,
                &err3)) {
            /* report error */
            error_msg("Failed to load default settings: %d: %s",
                    err3->code, err3->message);
            return FALSE;
        }
    }

    g_free(path);
    return TRUE;
}


/** \brief  Set a string setting at \a group -> \a key to \a value
 *
 * \param[in]   group   group
 * \param[in]   key     key
 * \param[in]   value   new value
 *
 * \return  TRUE on success
 */
gboolean settings_set_str(const char *group, const char *key, const char *value)
{
    debug_msg("This is supposed to set '%s/%s' to '%s'.", group, key, value);
    return TRUE;
}


/** \brief  Get a string setting from \a group with \a key
 *
 * \param[in]   group   group
 * \param[in]   key     key
 * \param[out]  value   result
 *
 * \return  TRUE on success
 */
gboolean settings_get_str(const char *group, const char *key, const char **value)
{
    gchar *s;
    GError *err = NULL;

    s = g_key_file_get_value(keyfile, group, key, &err);
    if (s == NULL) {
        debug_msg("oops: %d: %s", err->code, err->message);
        *value = NULL;
        return FALSE;
    }
    *value = s;
    return TRUE;
}



/** \brief  Get an integer setting from \a group with \a key
 *
 * \param[in]   group   group
 * \param[in]   key     key
 * \param[out]  value   result
 *
 * \return  TRUE on success
 */
gboolean settings_get_int(const char *group, const char *key, int *value)
{
    GError *err = NULL;
    int i;

    i = g_key_file_get_integer(keyfile, group, key, &err);
    if (i == 0 && err->code != 0) {
        debug_msg("oops: %d: %s", err->code, err->message);
        return FALSE;
    }
    *value = i;
    return TRUE;
}


/** \brief  Initialize the settings system
 *
 * Check/create the settings dir and file.
 *
 * \return  TRUE on success
 */
gboolean settings_init(void)
{
    keyfile = NULL;

    if (!settings_create_dir()) {
        error_msg("~/.config/gtk3vicemon/ is not a directory or could not be"
                " created.");
        return FALSE;
    }

    keyfile = g_key_file_new();
    if (!settings_read()) {
        error_msg("OOPS");
        return FALSE;
    }

    gsize length = 0;
    GError *err = NULL;
    gchar **keys = g_key_file_get_keys(keyfile, "VICE", &length, &err);
    debug_msg("Dumping section [VICE]:");
    for (gsize i = 0; i < length; i++) {
        g_print("%-10s-> %s\n", keys[i],
                g_key_file_get_string(keyfile, "VICE", keys[i], &err));
    }

    return TRUE;
}


void settings_exit(void)
{
    if (keyfile != NULL) {
        g_key_file_free(keyfile);
    }
}

