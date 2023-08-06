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
#include "CANVAS.HPP"
#include "COLORING.HPP"
#ifndef STATUSBAR_H
#define STATUSBAR_H

class StatusBar {
	const int statusBarYTolerance = 2;

	bool atTop;
	Canvas* canvas;
	Coloring* coloring;

	public:
		StatusBar(Canvas* canvasRef, Coloring* coloringRef) :
			atTop(false)
			{
				this->canvas = canvasRef;
				this->coloring = coloringRef;
			}
		bool getAtTop();
		void updateStatusBar();
};

#endif
