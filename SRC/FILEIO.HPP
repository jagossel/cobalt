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
#include "CANVAS.HPP"
#include "COLORING.HPP"
#include "STATBAR.HPP"
#include "UIHNDLR.HPP"

#ifndef FILEIO_H
#define FILEIO_H

#define FILE_HEADER "COBALTV2"

#define FILE_EXISTS_MESSAGE(fileName) "File, " + fileName + ", exists.  Overwrite?"
#define FILE_NOT_FOUND_MESSAGE(fileName) "File not found: " + fileName

class FileHandler {
	bool v1DrawingLoaded;
	Canvas* canvas;
	Coloring* coloring;
	StatusBar* statusBar;
	UiHandler* uiHandler;

	public:
		FileHandler(Canvas* canvasRef, Coloring* coloringRef, StatusBar* statusBarRef, UiHandler* uiHandlerRef)
			: v1DrawingLoaded(false)
		{
			this->canvas = canvasRef;
			this->coloring = coloringRef;
			this->statusBar = statusBarRef;
			this->uiHandler = uiHandlerRef;
		}

		void newDrawing();
		void saveDrawing();
		void loadDrawing();
		void exportDrawing();
		void importDrawing();
		bool drawingExists(std::string fileName);
};

#endif
