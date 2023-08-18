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
#include "EXCEPTS.HPP"
#include "FILEIO.HPP"
#include "V2LDR.HPP"

// File Layout:
//     8 bytes: File Header = COBALTV2
//     1 byte : Minimum Cursor Y
//     1 byte : Maximum Cursor Y
//     1 byte : Minimum Cursor X
//     1 byte : Maximum Cursor X
// 6,000 bytes: 25 (Height) * 80 (Width) * <Drawing Data> (3 bytes)
// NOTE: The file runs along with width (X) first before the height (Y).
//
// Drawing Data Layout:
// 1 byte: Character Code
// 1 byte: Foreground Color Code
// 1 byte: Background Color Code
//
// Total:
// 8 + 1 + 1 + 1 + 1 + (25 * 80 * 3) =
// 12 + 6000 = 6012
std::vector<DrawingCharacter> DrawingLoaderVersion2::loadDrawing() {
	int fileIndex = 0;

	// Check 4: Does it have the valid file header?
	std::string expectedFileHeader = FILE_HEADER;
	std::string actualFileHeader = "";
	int headerLength = expectedFileHeader.length();
	for (int i = 0; i < headerLength; i++) {
		fileIndex++;
		actualFileHeader += this->buffer->at(i);
	}

	if (actualFileHeader != expectedFileHeader) throw InvalidFileFormat();

	// Check 5: Are the dimensions matching?
	int minCurY = this->buffer->at(fileIndex++);
	int maxCurY = this->buffer->at(fileIndex++);
	int minCurX = this->buffer->at(fileIndex++);
	int maxCurX = this->buffer->at(fileIndex++);
	bool validDimensions =
		minCurY == this->minimumCursorY
		&& maxCurY == this->maximumCursorY
		&& minCurX == this->minimumCursorX
		&& maxCurX == this->maximumCursorX;

	if (!validDimensions) throw InvalidFileFormat();

	int bufferSize = this->buffer->size();
	std::vector<DrawingCharacter> drawing;
	while (fileIndex < bufferSize) {
		char characterCode = this->buffer->at(fileIndex++);
		char foregroundColor = this->buffer->at(fileIndex++);
		if (foregroundColor > 15) throw InvalidFileFormat();

		char backgroundColor = this->buffer->at(fileIndex++);
		if (backgroundColor > 15) throw InvalidFileFormat();

		DrawingCharacter drawingCharacter;
		drawingCharacter.character = characterCode;
		drawingCharacter.foregroundColor = foregroundColor;
		drawingCharacter.backgroundColor = backgroundColor;

		drawing.push_back(drawingCharacter);
	}

	return drawing;
}
