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
#ifndef MACROS_H
#define MACROS_H

// Common Strings
#define INVALID_FILE_MESSAGE "Invalid file format."

// Common Calculations
#define COLOR_PAIR_INDEX(fgc, bgc) ((bgc * 16) + fgc)
#define CANVAS_INDEX(y, x, maxX) ((y * (maxX + 1)) + x)

#endif
