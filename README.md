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

