# Gtk3 VICE Monitor

An attempt at creating a 'reference implementation' of a standalone monitor
for VICE, using the binary monitor interface. The UI toolkit selected is Gtk3,
since I have some experience with it when building the Gtk3 UI for VICE.


## Installation

In order to build this tool, you'll need a proper C99-capable compiler (so not
Visual Studio), auto tools, Gtk3 libs and some related tools.


### Dependencies

*Note: Some package names are how Debian names its packages, but they should be fairly easily to translate to other Linux distribution, \*BSD or MSYS2.*

* autotools (autoconf, automake, GNU make)
* libgtk-3-dev
* libglib-2.0-dev
* libglib-2.0-dev-bin (provides glib-compile-resources)
* xmllint (requirement for glib-compile-resources)


### Set up for compilation

First we need to generate configure and its dependencies:
```bash
$ autoreconf --install
```
That will create ./configure and the scripts required by ./configure.

The we run:
```bash
$ ./configure
```
To configure the tool and search for all dependencies. Once all dependencies are
satified, we can continue.

### Build the tool

Once that succeeds, we can run:
```bash
$ make
```
To build the tool, which will be `./src/gtk3vicemon`.

