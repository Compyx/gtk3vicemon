/** \file   debug.h
 * \brief   Debug messages, dependent on configure option --enable-debug
 *
 * This module provides some macros to be used to print debugging output on
 * stdout when --enable-debug was passed in configure.
 *
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

/*
 * This file is part of Gtk3 Vice Montitor
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#ifndef DEBUG_H
# define DEBUG_H

#include "config.h"

# include <stdio.h>
# include <stdlib.h>
# include <gtk/gtk.h>
# include <glib.h>


/** \def debug_msg
 * \brief  Print debugging info on stdout
 *
 * Works just like g_print() and printf(), except that every line is prefixed
 * with "[debug] $FILE:$LINE::$FUNC(): ".
 * This macro outputs a newline. so the user should not provide one in the
 * message, unless an extra newline is preferred.
 */

/** \def NOT_IMPLEMENTED_WARN_ONLY
 * \brief  Not-implemented message with file, function and lineno, only warns
 */

/** \def NOT_IMPLEMENTED_WARN_X_TIMES
 * \brief  Not-implemented message, shown at most X times
 *
 * This macro limits the number of 'not implemented' messages appearing on
 * stderr, so the terminal debug output doesn't get flooded.
 *
 * Usage: declare a `static int foo_msgs` somewhere and then in the function you
 *        want to limit the number of messages it spits out, use this macro:
 *        NOT_IMPLEMENTED_WARN_X_TIMES(foo_msgs, 5);    (warn 5 times at most)
 *
 * \param[in,out]   C   counter variable (int)
 * \param[in]       X   maximum number of times to show the warning (int)
 */

/** \def NOT_IMPLEMENTED
 * \brief  Not-implemented message with file, function and lineno, calls exit(1)
 *
 * This one should NOT depend on HAVE_DEBUG since it calls exit(1), to
 * avoid a 'mysterious' exit/crash without any information.
 */

/** \def INCOMPLETE_IMPLEMENTATION
 * \brief  Incomplete implementation message, only warns
 */

/** \def TEMPORARY_IMPLEMENTATION
 * \brief  Temporary implementation message, only warns
 */


/* HAVE_DEBUG comes from configure */
# ifdef HAVE_DEBUG

#  define debug_msg(...) \
    g_print("[debug] %s:%d::%s(): ", __FILE__, __LINE__, __func__); \
    g_print(__VA_ARGS__); \
    g_print("\n");

#  define NOT_IMPLEMENTED_WARN_ONLY() \
    fprintf(stderr, \
            "%s:%d: warning: function %s() is not implemented yet, continuing\n", \
            __FILE__, __LINE__, __func__)

#  define NOT_IMPLEMENTED_WARN_X_TIMES(C, X) \
    if ((C)++ < (X)) { \
        fprintf(stderr, \
                "%s:%d: warning function %s() is not implemented yet, " \
                "warning %d/%d\n", \
                __FILE__, __LINE__, __func__, (C), (X)); \
    }

#  define INCOMPLETE_IMPLEMENTATION() \
    fprintf(stderr, \
            "%s:%d: warning: function %s() is not fully implemented yet, continuing\n", \
            __FILE__, __LINE__, __func__)


#  define TEMPORARY_IMPLEMENTATION() \
    fprintf(stderr, \
            "%s:%d: warning: function %s() contains a temporary implementation, continuing\n", \
            __FILE__, __LINE__, __func__)


# else  /* HAVE_DEBUG */

/*  Empty placeholders */
#  define debug_msg(...)
#  define NOT_IMPLEMENTED_WARN_ONLY()
#  define NOT_IMPLEMENTED_WARN_X_TIMES(C, X)
#  define INCOMPLETE_IMPLEMENTATION()
#  define TEMPORARY_IMPLEMENTATION()

# endif /* HAVE DEBUG */


# define NOT_IMPLEMENTED() \
    fprintf(stderr, \
            "%s:%d: error: function %s() is not implemented yet, exiting\n", \
            __FILE__, __LINE__, __func__); \
    exit(1)


# define error_msg(...) \
    g_printerr("[Error] %s:%d::%s(): ", __FILE__, __LINE__, __func__); \
    g_printerr(__VA_ARGS__); \
    g_printerr("\n");



#endif  /* DEBUG_H */
