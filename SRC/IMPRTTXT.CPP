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
#include "IMPRTTXT.HPP"

std::vector<DrawingCharacter> TextDrawingImporter::importDrawing(std::vector<char>* fileBuffer) {
	this->createBlankCanvas();

	const int foregroundColor = 7;
	const int backgroundColor = 0;

	auto tryAddReadCharacter = [&] (char readCharacter) {
		DrawingCharacter drawingCharacter;
		drawingCharacter.character = readCharacter;
		drawingCharacter.foregroundColor = foregroundColor;
		drawingCharacter.backgroundColor = backgroundColor;

		return this->tryAddCharacter(drawingCharacter);
	};

	int fileSize = fileBuffer->size();
	for (int i = 0; i < fileSize; i++) {
		char readChar = fileBuffer->at(i);
		if (0x0A == readChar) {        // Line Feed
			if (!this->tryAdvanceLine()) break;
		} else if (0x0D == readChar) { // Carriage Return
			this->currentCursorX = this->minimumCursorX;
		} else if (0x1A == readChar) { // End of File
			break;
		} else if (0x1B == readChar) { // Escape
			tryAddReadCharacter(' '); // Replace the escape key with a space
		} else if (!tryAddReadCharacter(readChar)) break;
	}

	return this->drawing;
}
