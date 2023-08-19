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
#include "V1LDR.CPP"
#include "V2LDR.CPP"
#include "MACROS.HPP"

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
		bool continueSaving =
			!this->v1DrawingLoaded
			|| this->uiHandler->getConfirmation("Saving will upgrade the drawing to V2.  Continue?");

		if (continueSaving) {
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
}

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

	int minimumCursorY = this->canvas->getMinimumCursorY();
	int maximumCursorY = this->canvas->getMaximumCursorY();
	int minimumCursorX = this->canvas->getMinimumCursorX();
	int maximumCurosrX = this->canvas->getMaximumCursorX();

	// Check 3: Is it the expected file size?
	// Given how inflexible Cobalt is when it comes to dimensions, both
	// versions have fixed file sizes.  It is possible to get away with
	// using their file sizes as a way to check.
	int version1ExpectedFileSize = 6000;
	int version2ExpectedFileSize = 6012;
	int actualFileSize = fileBuffer.size();
	if (actualFileSize == version1ExpectedFileSize) {
		try {
			this->v1DrawingLoaded = true;
			DrawingLoaderVersion1 loader(
				&fileBuffer,
				minimumCursorY,
				maximumCursorY,
				minimumCursorX,
				maximumCurosrX);

			this->canvas->loadDrawing(loader.loadDrawing());
		} catch (const std::exception& ex) {
			this->uiHandler->showErrorMessage(ex.what());
		}
	} else if (actualFileSize == version2ExpectedFileSize) {
		try {
			this->v1DrawingLoaded = false;
			DrawingLoaderVersion2 loader(
				&fileBuffer,
				minimumCursorY,
				maximumCursorY,
				minimumCursorX,
				maximumCurosrX
			);

			this->canvas->loadDrawing(loader.loadDrawing());
		} catch (const std::exception& ex) {
			this->uiHandler->showErrorMessage(ex.what());
		}
	} else {
		this->uiHandler->showErrorMessage(INVALID_FILE_MESSAGE);
		return;
	}
}

bool FileHandler::drawingExists(std::string fileName) {
	bool fileExists = false;
	if (FILE* fileHandle = fopen(fileName.c_str(), "r")) {
		fileExists = true;
		fclose(fileHandle);
	}

	return fileExists;
}
