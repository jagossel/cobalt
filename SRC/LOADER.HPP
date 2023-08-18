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
#include <vector>
#include "DRAWCHAR.HPP"

#ifndef LOADER_H
#define LOADER_H

class DrawingLoader {
	protected:
		std::vector<char>* buffer;
		int minimumCursorY;
		int maximumCursorY;
		int minimumCursorX;
		int maximumCursorX;
	public:
		DrawingLoader(std::vector<char>* bufferRef, int minCurY, int maxCurY, int minCurX, int maxCurX) {
			this->buffer = bufferRef;
			this->minimumCursorY = minCurY;
			this->maximumCursorY = maxCurY;
			this->minimumCursorX = minCurX;
			this->maximumCursorX = maxCurX;
		}
		virtual std::vector<DrawingCharacter> loadDrawing() = 0;
};

#endif
