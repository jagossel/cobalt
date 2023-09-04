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
#include "IMPRTANS.HPP"
#include "MACROS.HPP"

// Currently, there is only support for the color commands; pretty much the same ones that Cobalt
// can export.
std::vector<DrawingCharacter> AnsiDrawingImporter::importDrawing(std::vector<char>* fileBuffer) {
	std::string currentAnsiCommand = "";

	int currentForeground = 7;
	int currentBackground = 0;
	bool enteringEscapeSequence = false;
	bool readingAnsiCommand = false;
	bool addIntensity = false;

	auto resetEscape = [&] () {
		if (!enteringEscapeSequence) return;
		enteringEscapeSequence = false;
	};

	auto parseAnsiColorCommandPart = [&] (std::string ansiCommandPart) {
		int commandPartSize = ansiCommandPart.length();
		if (1 == commandPartSize) {
			if ("0" == ansiCommandPart) {
				addIntensity = false;
				currentForeground = 7;
				currentBackground = 0;
			} else if ("1" == ansiCommandPart) {
				addIntensity = true;
				if (currentForeground < 8) {
					currentForeground += 8;
				}
			}
		} else if (2 == commandPartSize) {
			char colorType = ansiCommandPart.at(0);
			char colorCode = ansiCommandPart.at(1);
			if ('3' == colorType) {        // Foreground Color
				currentForeground = colorCode - 0x30;
				if (addIntensity) {
					currentForeground += 8;
				}
			} else if ('4' == colorType) { // Background Color
				currentBackground = colorCode - 0x30;
			}
		}
	};

	auto parseAnsiColorCommand = [&] () {
		std::string ansiCommandPart = "";
		int ansiCommandLength = currentAnsiCommand.length();
		for (int i = 0; i < ansiCommandLength; i++) {
			char ansiCommandChar = currentAnsiCommand.at(i);
			if (0x3B == ansiCommandChar) {
				parseAnsiColorCommandPart(ansiCommandPart);
				ansiCommandPart = "";
			} else {
				ansiCommandPart += ansiCommandChar;
			}
		}

		parseAnsiColorCommandPart(ansiCommandPart);
		currentAnsiCommand = "";
	};

	auto tryAddReadCharacter = [&] (char readCharacter) {
		DrawingCharacter drawingCharacter;
		drawingCharacter.character = readCharacter;
		drawingCharacter.foregroundColor = currentForeground;
		drawingCharacter.backgroundColor = currentBackground;

		return this->tryAddCharacter(drawingCharacter);
	};

	this->createBlankCanvas();

	int fileLength = fileBuffer->size();
	for (int i = 0; i < fileLength; i++) {
		char readChar = fileBuffer->at(i);
		if (0x0A == readChar) {                                                  // Line Feed
			resetEscape();
			if (!this->tryAdvanceLine()) break;
		} else if (0x0D == readChar) {                                           // Carriage Return
			resetEscape();
			this->currentCursorX = this->minimumCursorX;
		} else if (0x1A == readChar) {                                           // End of File
			resetEscape();
			break;
		} else if (0x1B == readChar) {                                           // Escpae
			enteringEscapeSequence = true;
		} else if ((readChar >= 0x30 && readChar <= 0x39) || 0x3B == readChar) { // 0 - 9 or ;
			if (readingAnsiCommand) {
				currentAnsiCommand += readChar;
			} else {
				resetEscape();
				if (!tryAddReadCharacter(readChar)) break;
			}
		} else if (0x5B == readChar) {                                           // [
			if (enteringEscapeSequence) {
				readingAnsiCommand = true;
				resetEscape();
			} else {
				resetEscape();
				if (!tryAddReadCharacter(readChar)) break;
			}
		} else if (0x6D == readChar) {                                           // m
			if (readingAnsiCommand) {
				readingAnsiCommand = false;
				resetEscape();
				parseAnsiColorCommand();
			} else {
				resetEscape();
				if (!tryAddReadCharacter(readChar)) break;
			}
		} else {
			resetEscape();
			if (!tryAddReadCharacter(readChar)) break;
		}
	}

	return this->drawing;
}
