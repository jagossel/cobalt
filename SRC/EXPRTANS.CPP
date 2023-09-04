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
#include <cstdio>
#include <string>
#include <vector>
#include "DRAWCHAR.HPP"
#include "EXCEPTS.HPP"
#include "EXPRTANS.HPP"

void AnsiDrawingExporter::exportDrawing(std::string fileName) {
	int currentForegroundColor = -1;
	int currentBackgroundColor = -1;

	if (FILE* fileHandle = fopen(fileName.c_str(), "wb")) {
		int drawingLength = this->drawing->size();
		for (int i = 0; i < drawingLength; i++) {
			DrawingCharacter drawingCharacter = this->drawing->at(i);
			int foregroundColor = drawingCharacter.foregroundColor;
			int backgroundColor = drawingCharacter.backgroundColor;

			// Background Intensity is not supported in ANSI; ignore it
			bool insertAddIntensityCode = foregroundColor > 7 && currentForegroundColor < 8;
			bool insertRemoveIntensityCode = foregroundColor < 8 && currentForegroundColor > 7;
			bool changeForegroundColor = foregroundColor != currentForegroundColor;
			bool changeBackgroundColor = backgroundColor != currentBackgroundColor;
			bool addEscapeSequence =
				insertAddIntensityCode
				|| insertRemoveIntensityCode
				|| changeForegroundColor
				|| changeBackgroundColor;

			if (addEscapeSequence) {
				// Adds "^[[" (Escape, Open Bracket)
				fputc(0x1B, fileHandle);
				fputc(0x5B, fileHandle);

				if (insertAddIntensityCode) {
					// Adds "1;"
					fputc(0x31, fileHandle);
					fputc(0x3B, fileHandle);
				} else if (insertRemoveIntensityCode) {
					// Adds "0;"
					fputc(0x30, fileHandle);
					fputc(0x3B, fileHandle);

					// For re-adding the color codes since ^[[0m (Escape, Open Square Bracket, 0) resets
					// the attributes for both bold and color.
					changeForegroundColor = true;
					changeBackgroundColor = true;
				}

				if (changeForegroundColor) {
					// Adds "30" through "37"
					char foregroundColorCode = (foregroundColor % 8) + 0x30;
					fputc(0x33, fileHandle);
					fputc(foregroundColorCode, fileHandle);
				}

				if (changeBackgroundColor) {
					if (changeForegroundColor) {
						// Adds ";"
						fputc(0x3B, fileHandle);
					}

					// Adds "40" through "47"
					char backgroundColorCode = (backgroundColor % 8) + 0x30;
					fputc(0x34, fileHandle);
					fputc(backgroundColorCode, fileHandle);
				}

				// Terminate the escape sequence with "m"
				fputc(0x6D, fileHandle);

				// Update the current foreground and background colors to prevent adding
				// unneccessiary escape sequences.
				currentForegroundColor = foregroundColor;
				currentBackgroundColor = backgroundColor;
			}

			char characterCode = drawingCharacter.character;
			if (0x1A == characterCode || 0x1B == characterCode) {
				// The EOF character could end the file prematurely or the Escape character could fowl up the
				// codes; replace these two with a space character.
				characterCode = 0x20;
			}

			fputc(characterCode, fileHandle);
		}

		// Done writing the file; write a reset code escape sequence: ^[[0m (Escape, Open Square Bracket, 0, m)
		fputc(0x1B, fileHandle);
		fputc(0x5B, fileHandle);
		fputc(0x30, fileHandle);
		fputc(0x6D, fileHandle);

		fclose(fileHandle);
	} else throw UnableToOpenFile();
}
