# Cobalt (Rewrite)

Welcome to Cobalt, an ASCII art editor for MS-DOS.  This has been completely
re-written in C++ using DJGPP.

## Compiling

In order to build Cobalt in MS-DOS, DJGPP for MS-DOS is required.  Visit
https://www.delorie.com/djgpp/ select "Zip Picker" with the following
options defined:
 - FTP Site: http://www.delorie.com
 - Build and run programs with DJGPP
 - Operating System: MS-DOS, OpenDOS, PC-DOS
 - On-line documentation: Yes
 - Programming Languages: C, C++
 - IDE: RHIDE
 - GDB: Yes
 - Toolkits: Unix Curses Emulator

Click on the "Tell me which files I need" and follow the instructions.

Once DJGPP has been installed, compile Cobalt:
```
CD SRC
MAKE
```

The compiled version of Cobalt will be in the BIN folder.

## Editing and Testing

This application was written using Visual Studio Code configured to using the
GNU C/C++ tool chain, and compiled and tested in DOSBox version 0.74-3 using
DJGPP.

## Exporting drawings to ANSI or plain text

While the application is capable of using EOF (end-of-file) or Escape
characters and 16 background colors, there is very limited support in the
DOSBox implementation of the ANSI.SYS driver; background colors will not
add in the intensity and it will replace the EOF/Escape characters with a
space.  The drawing have not been tested in MS-DOS's nor FreeDOS's ANSI.SYS
drivers.

Plain-text files are stripped of all the colors, and that character of the
line will be removed so that it can display in the 80x25 console correctly.
