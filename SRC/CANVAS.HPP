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
#include "DRAWCHAR.HPP"
#include "COLORING.HPP"
#ifndef CANVAS_H
#define CANVAS_H

class Canvas {
	const int minimumCharacterSetIndex = 0;
	const int maximumCharacterSetIndex = 9;
	const char characterSets[10][10] =
	{
		{ '\xB0', '\xB1', '\xB2', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF', '\xFE', '\x20' },
		{ '\xDA', '\xBF', '\xC4', '\xC2', '\xC1', '\xC3', '\xB4', '\xB3', '\xC0', '\xD9' },
		{ '\xC9', '\xBB', '\xCD', '\xCB', '\xCA', '\xCC', '\xB9', '\xBA', '\xC8', '\xBC' },
		{ '\xD6', '\xB7', '\xC4', '\xD2', '\xD0', '\xC7', '\xB6', '\xBA', '\xD3', '\xBD' },
		{ '\xD5', '\xB8', '\xCD', '\xD1', '\xCF', '\xC6', '\xB5', '\xB3', '\xD4', '\xBE' },
		{ '\xC5', '\xCE', '\xD7', '\xD8', '\xF0', '\xF4', '\xF5', '\xAE', '\xAF', '\xFE' },
		{ '\x2E', '\xF9', '\xFA', '\xF8', '\x2A', '\x0F', '\xF0', '\xF1', '\xF2', '\xF3' },
		{ '\xF4', '\xF5', '\xF6', '\xF7', '\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD' },
		{ '\x1E', '\x1F', '\x10', '\x11', '\x12', '\x1D', '\x18', '\x19', '\x1A', '\x1B' },
		{ '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x0B', '\x0C', '\x0F', '\x20' }
	};

	int minimumCursorX;
	int currentCursorX;
	int maximumCursorX;

	int minimumCursorY;
	int currentCursorY;
	int maximumCursorY;

	int currentCharacterSetIndex;

	DrawingCharacter drawingCanvas[25][80];
	Coloring* coloring;

	public:
		Canvas(Coloring* coloringRef) :
			minimumCursorX(0),
			currentCursorX(0),
			maximumCursorX(0),
			minimumCursorY(0),
			currentCursorY(0),
			maximumCursorY(0),
			currentCharacterSetIndex(0) { this->coloring = coloringRef; }
		void resizeCanvas();
		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();
		void moveToHome();
		void moveToEnd();
		void moveToNextLine();
		void previousCharacterSet();
		void nextCharacterSet();
		void reset();
		int getMinimumCursorX();
		int getCurrentCursorX();
		int getMaximumCursorX();
		int getMinimumCursorY();
		int getCurrentCursorY();
		int getMaximumCursorY();
		int getCurrentCharacterSetIndex();
		char getCharacterFromSet(int characterIndex);
		void drawCharacterFromSet(int characterIndex);
		void drawTypedCharacter(char typedCharacter);
		void resetCharacter();
		void showFullDrawing();
		void redrawAll();
		void redrawRegion(int startX, int startY, int endX, int endY);
		void initializeDrawing();
};

#endif
