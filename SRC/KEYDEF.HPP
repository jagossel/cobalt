/***
 * Cobalt (C++ Rewrite) - ASCII Art
 * Copyright (C) 2018-2023  Jonathon Gosselin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ***/
#ifndef KEYDEF_H
#define KEYDEF_H

// There are some keys that are not working in PDCurses; here, we are going to define our own.
#define COBALT_KEY_SHIFT_F2  KEY_F(14)
#define COBALT_KEY_SHIFT_F3  KEY_F(15)
#define COBALT_KEY_CTL_F     0x06
#define COBALT_KEY_CTL_N     0x0E
#define COBALT_KEY_CTL_O     0x0F
#define COBALT_KEY_CTL_S     0x13

#define COBALT_KEY_BACKSPACE 0x08
#define COBALT_KEY_ENTER     0x0D
#define COBALT_KEY_ESCAPE    0x1B

#endif
