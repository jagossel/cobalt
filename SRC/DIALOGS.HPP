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
#include <CURSES.H>
#include "CANVAS.HPP"

#ifndef DIALOGS_H
#define DIALOGS_H

#define DIALOG_CENTER_START(pw, tw) ((pw - tw) / 2)

class Dialogs {
	Canvas* canvas;
	public:
		Dialogs(Canvas* canvasRef) {
			this->canvas = canvasRef;
		}
		WINDOW* createDialog(int windowWith, int windowHeight);
		void destroyDialog(WINDOW* dialog);
};

#endif
