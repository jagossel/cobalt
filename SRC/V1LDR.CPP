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
#include "EXCEPTS.HPP"
#include "FILEIO.HPP"
#include "V1LDR.HPP"

// File Layout:
// 6,000 bytes: 80 (Width) * 25 (Height) * <Drawing Data> (3 bytes)
// NOTE: The file runs along the height (Y) first, before the width (X).
// NOTE: There needs to be a color remaping as well.
//
// Drawing Data Layout:
// 1 byte: Character Code
// 1 byte: Foreground Color Code
// 1 byte: Background Color Code
std::vector<DrawingCharacter> DrawingLoaderVersion1::loadDrawing() {
	auto convertColorCode = [&] (char basicColorCode) {
		char ansiColorCode = 0;
		switch (basicColorCode) {
			case 1:
				ansiColorCode = 4;
				break;
			case 3:
				ansiColorCode = 6;
				break;
			case 4:
				ansiColorCode = 1;
				break;
			case 6:
				ansiColorCode = 3;
				break;
			case 9:
				ansiColorCode = 12;
				break;
			case 11:
				ansiColorCode = 14;
				break;
			case 12:
				ansiColorCode = 9;
				break;
			case 14:
				ansiColorCode = 11;
				break;
			default:
				ansiColorCode = basicColorCode;
				break;
		}

		return ansiColorCode;
	};

	std::vector<DrawingCharacter> drawing;
	for (int loadY = 0; loadY < 25; loadY++) {
		for (int loadX = 0; loadX < 80; loadX++) {
			int offset = ((loadX * 25) + loadY) * 3;

			DrawingCharacter drawingCharacter;
			drawingCharacter.character = this->buffer->at(offset);
			drawingCharacter.foregroundColor = convertColorCode(this->buffer->at(offset + 1));
			drawingCharacter.backgroundColor = convertColorCode(this->buffer->at(offset + 2));

			drawing.push_back(drawingCharacter);
		}
	}

	return drawing;
}
