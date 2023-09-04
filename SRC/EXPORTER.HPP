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
#include <string>
#include <vector>
#include "DRAWCHAR.HPP"

#ifndef EXPORTER_H
#define EXPORTER_H

class DrawingExporter {
	protected:
		std::vector<DrawingCharacter>* drawing;
		int minimumCursorY;
		int maximumCursorY;
		int minimumCursorX;
		int maximumCursorX;
	public:
		DrawingExporter(
			std::vector<DrawingCharacter>* drawingRef,
			int minCurY,
			int maxCurY,
			int minCurX,
			int maxCurX) {
				this->drawing = drawingRef;
				this->minimumCursorY = minCurY;
				this->maximumCursorY = maxCurY;
				this->minimumCursorX = minCurX;
				this->maximumCursorX = maxCurX;
			}
		virtual void exportDrawing(std::string fileName) = 0;
};

#endif
