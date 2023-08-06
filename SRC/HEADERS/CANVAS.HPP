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
#ifndef CANVAS_H
#define CANVAS_H

class Canvas {
	const int minimumForegroundColor = 0;
	const int maximumForegroundColor = 15;

	const int minimumBackgroundColor = 0;
	const int maximumBackgroundColor = 15;

	int minimumCursorX;
	int currentCursorX;
	int maximumCursorX;

	int minimumCursorY;
	int currentCursorY;
	int maximumCursorY;

	int currentForegroundColor;
	int currentBackgroundColor;

	public:
		Canvas() :
			minimumCursorX(0),
			currentCursorX(0),
			maximumCursorX(0),
			minimumCursorY(0),
			currentCursorY(0),
			maximumCursorY(0),
			currentForegroundColor(7),
			currentBackgroundColor(0) { }
		void resizeCanvas();
		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();
		void moveToHome();
		void moveToEnd();
		void moveToNextLine();
		void previousForegroundColor();
		void nextForegroundColor();
		void previousBackgroundColor();
		void nextBackgroundColor();
		void reset();
		int getMinimumCursorX();
		int getCurrentCursorX();
		int getMaximumCursorX();
		int getMinimumCursorY();
		int getCurrentCursorY();
		int getMaximumCursorY();
		int getMinimumForegroundColor();
		int getCurrentForegroundColor();
		int getMaximumForegroundColor();
		int getMinimumBackgroundColor();
		int getCurrentBackgroundColor();
		int getMaximumBackgroundColor();
};

#endif
