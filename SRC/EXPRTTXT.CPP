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
#include "EXPRTTXT.HPP"
#include "MACROS.HPP"
#include "EXCEPTS.HPP"

void TextDrawingExporter::exportDrawing(std::string fileName) {
	if (FILE* fileHandle = fopen(fileName.c_str(), "wb")) {
		for (int y = this->minimumCursorY; y < this->maximumCursorY; y++) {
			for (int x = this->minimumCursorX; x < this->maximumCursorX; x++) {
				int drawingIndex = CANVAS_INDEX(y, x, this->maximumCursorX);
				DrawingCharacter drawingCharacter = this->drawing->at(drawingIndex);
				char characterCode = drawingCharacter.character;
				if (0x1A == characterCode || 0x1B == characterCode) {
					// The EOF character could end the file prematurely or the Escape character could fowl up the
					// codes; replace these two with a space character.
					characterCode = 0x20;
				}

				fputc(characterCode, fileHandle);
			}

			// Add CR/LF characters at the end of the line.
			fputc(0x0D, fileHandle);
			fputc(0x0A, fileHandle);
		}

		fclose(fileHandle);
	} else throw UnableToOpenFile();
}
