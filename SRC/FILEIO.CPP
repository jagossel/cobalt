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
	std::string fileName = this->uiHandler->getFileName();
	if (fileName.length() <= 0) return;
	fileName += ".CPD";

	std::vector<DrawingCharacter>* drawing = this->canvas->getDrawing();
	int drawingSize = drawing->size();

	bool fileExists = this->drawingExists(fileName);

	std::string confirmOverwriteMessage = "File, " + fileName + ", exists.  Overwrite?";
	bool confirmOverwrite = !fileExists || this->uiHandler->getConfirmation(confirmOverwriteMessage);
	if (confirmOverwrite) {
		FILE* fileHandle = fopen(fileName.c_str(), "wb");
		if (NULL != fileHandle) {
			fprintf(fileHandle, FILE_HEADER);
			fputc(this->canvas->getMinimumCursorY(), fileHandle);
			fputc(this->canvas->getMaximumCursorY(), fileHandle);
			fputc(this->canvas->getMinimumCursorX(), fileHandle);
			fputc(this->canvas->getMaximumCursorX(), fileHandle);
			for (int i = 0; i < drawingSize; i++) {
				DrawingCharacter drawingCharacter = drawing->at(i);
				unsigned char character = drawingCharacter.character;
				unsigned char foregroundColor = drawingCharacter.foregroundColor;
				unsigned char backgroundColor = drawingCharacter.backgroundColor;

				fputc(character, fileHandle);
				fputc(foregroundColor, fileHandle);
				fputc(backgroundColor, fileHandle);
			}

			fclose(fileHandle);

			std::string message = "Drawing saved: " + fileName;
			this->uiHandler->showMessage(message);
		} else {
			this->uiHandler->showErrorMessage("Cannot save drawing.");
		}
	}
}

// TODO: Add backwards compatability to the older FreeBASIC version of Cobalt v1.0+
// TODO: Resize the screen instead of checking the dimensions
void FileHandler::loadDrawing() {
	std::string fileName = this->uiHandler->getFileName();
	if (fileName.length() <= 0) return;
	fileName += ".CPD";

	// Check 1: Does the file exists?
	if (!this->drawingExists(fileName)) {
		std::string notFoundMessage = "File not found: " + fileName;
		this->uiHandler->showErrorMessage(notFoundMessage);
		return;
	}

	// Check 2: Can the file be opened?
	FILE* fileHandle = fopen(fileName.c_str(), "rb");
	if (NULL == fileHandle) {
		this->uiHandler->showErrorMessage("Cannot load drawing.");
		return;
	}

	std::vector<char> fileBuffer;
	while (!feof(fileHandle)) {
		fileBuffer.push_back(fgetc(fileHandle));
	}

	fclose(fileHandle);
	fileBuffer.pop_back();

	// File Layout:
	//     8 bytes: File Header = COBALTV2
	//     1 byte : Minimum Cursor Y
	//     1 byte : Maximum Cursor Y
	//     1 byte : Minimum Cursor X
	//     1 byte : Maximum Cursor X
	// 6,000 bytes: 80 (Width) * 25 (Height) * <Drawing Data> (3 bytes)
	//
	// Drawing Data Layout:
	// 1 byte: Character Code
	// 1 byte: Foreground Color Code
	// 1 byte: Background Color Code
	//
	// Total:
	// 8 + 1 + 1 + 1 + 1 + (80 * 25 * 3) =
	// 12 + 6000 = 6012
	//
	// Check 3: Is it the expected file size?
	int exepctedFileSize = 6012;
	int actualFileSize = fileBuffer.size();
	if (actualFileSize != exepctedFileSize) {
		this->uiHandler->showErrorMessage(INVALID_FILE_MESSAGE);
		return;
	}

	int fileIndex = 0;

	// Check 4: Does it have the valid file header?
	std::string expectedFileHeader = FILE_HEADER;
	std::string actualFileHeader = "";
	int headerLength = expectedFileHeader.length();
	for (int i = 0; i < headerLength; i++) {
		fileIndex++;
		actualFileHeader += fileBuffer.at(i);
	}

	if (actualFileHeader != expectedFileHeader) {
		this->uiHandler->showErrorMessage(INVALID_FILE_MESSAGE);
		return;
	}

	// Check 5: Are the dimensions matching?
	int minimumCursorY = fileBuffer.at(fileIndex++);
	int maximumCursorY = fileBuffer.at(fileIndex++);
	int minimumCursorX = fileBuffer.at(fileIndex++);
	int maximumCursorX = fileBuffer.at(fileIndex++);
	bool validDimensions =
		minimumCursorY == this->canvas->getMinimumCursorY()
		&& maximumCursorY == this->canvas->getMaximumCursorY()
		&& minimumCursorX == this->canvas->getMinimumCursorX()
		&& maximumCursorX == this->canvas->getMaximumCursorX();

	if (!validDimensions) {
		this->uiHandler->showErrorMessage(INVALID_FILE_MESSAGE);
		return;
	}

	std::vector<DrawingCharacter> drawing;
	while (fileIndex < exepctedFileSize) {
		char characterCode = fileBuffer.at(fileIndex++);
		char foregroundColor = fileBuffer.at(fileIndex++);
		if (foregroundColor > 15) {
			this->uiHandler->showErrorMessage(INVALID_FILE_MESSAGE);
			break;
		}

		char backgroundColor = fileBuffer.at(fileIndex++);
		if (backgroundColor > 15) {
			this->uiHandler->showErrorMessage(INVALID_FILE_MESSAGE);
			break;
		}

		DrawingCharacter drawingCharacter;
		drawingCharacter.character = characterCode;
		drawingCharacter.foregroundColor = foregroundColor;
		drawingCharacter.backgroundColor = backgroundColor;

		drawing.push_back(drawingCharacter);
	}

	this->canvas->loadDrawing(drawing);
}

bool FileHandler::drawingExists(std::string fileName) {
	bool fileExists = false;
	if (FILE* fileHandle = fopen(fileName.c_str(), "r")) {
		fileExists = true;
		fclose(fileHandle);
	}

	return fileExists;
}
