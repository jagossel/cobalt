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
#include "MACROS.HPP"

#ifndef IMPORTER_H
#define IMPORTER_H

class DrawingImporter {
	protected:
		std::vector<DrawingCharacter> drawing;
		int minimumCursorY;
		int currentCursorY;
		int maximumCursorY;
		int minimumCursorX;
		int currentCursorX;
		int maximumCursorX;
		void createBlankCanvas() {
			std::vector<DrawingCharacter> canvas;
			for (int initY = this->minimumCursorY; initY <= this->maximumCursorY; initY++) {
				for (int initX = this->minimumCursorX; initX <= this->maximumCursorX; initX++) {
					int index = CANVAS_INDEX(initY, initX, this->maximumCursorX);
					DrawingCharacter drawingCharacter;
					drawingCharacter.character = ' ';
					drawingCharacter.foregroundColor = 7;
					drawingCharacter.backgroundColor = 0;

					canvas.push_back(drawingCharacter);
				}
			}

			this->drawing = canvas;
		}
		bool tryAdvanceLine() {
			this->currentCursorY++;
			return this->currentCursorY <= this->maximumCursorY;
		}
		bool tryAdvanceCharacter() {
			bool continueProcessing = true;

			this->currentCursorX++;
			if (this->currentCursorX > this->maximumCursorX) {
				this->currentCursorX = this->minimumCursorX;
				continueProcessing = this->tryAdvanceLine();
			}

			return continueProcessing;
		}
		bool tryAddCharacter(DrawingCharacter drawingCharacter) {
			int index = CANVAS_INDEX(this->currentCursorY, this->currentCursorX, this->maximumCursorX);
			this->drawing.at(index) = drawingCharacter;
			return this->tryAdvanceCharacter();
		}
	public:
		DrawingImporter(
			int minCurY,
			int maxCurY,
			int minCurX,
			int maxCurX) :
				currentCursorX(0),
				currentCursorY(0) {
				this->minimumCursorY = minCurY;
				this->maximumCursorY = maxCurY;
				this->minimumCursorX = minCurX;
				this->maximumCursorX = maxCurX;
			}
		virtual std::vector<DrawingCharacter> importDrawing(std::vector<char>* fileBuffer) = 0;
};

#endif
