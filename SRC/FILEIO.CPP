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
#include <cstdio>
#include <CURSES.H>
#include "DRAWCHAR.HPP"
#include "FILEIO.HPP"

void FileHandler::newDrawing() {
	if (!this->uiHandler->getConfirmation("Start a new Drawing?")) return;

	canvas->reset();
	coloring->reset();
	canvas->initializeDrawing();
	move(canvas->getCurrentCursorX(), canvas->getCurrentCursorY());
}

void FileHandler::saveDrawing() {
	std::string fileName = this->uiHandler->getFileName("Save");
	if (fileName.length() <= 0) return;
	fileName = fileName + ".CPD";

	std::vector<DrawingCharacter>* drawing = this->canvas->getDrawing();
	int drawingSize = drawing->size();

	FILE* fileHandle = fopen(fileName.c_str(), "wb");
	for (int i = 0; i < drawingSize; i++) {
		// For backwards compabitlity with the original BASIC version, convert all
		// of this data into an unsigned char (should be a BYTE in FreeBASIC).
		DrawingCharacter drawingCharacter = drawing->at(i);
		unsigned char character = drawingCharacter.character;
		unsigned char foregroundColor = drawingCharacter.foregroundColor;
		unsigned char backgroundColor = drawingCharacter.backgroundColor;

		fputc(character, fileHandle);
		fputc(foregroundColor, fileHandle);
		fputc(backgroundColor, fileHandle);
	}

	fclose(fileHandle);
}
