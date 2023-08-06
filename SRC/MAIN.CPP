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
#include <curses.h>
#include "DRAWCHAR.HPP"
#include "CANVAS.CPP"
#include "COLORING.CPP"
#include "STATBAR.CPP"
#include "UIHNDLR.CPP"
#include "KEYHNDLR.CPP"
#include "FILEIO.CPP"

using namespace std;

int main() {
	initscr();
	raw();
	raw_output(true);
	keypad(stdscr, true);
	noecho();
	start_color();

	Coloring coloring;
	Coloring* coloringRef = &coloring;
	coloringRef->initColors();

	Canvas canvas(coloringRef);
	Canvas* canvasRef = &canvas;
	canvasRef->resizeCanvas();
	canvasRef->initializeDrawing();
	canvasRef->redrawAll();

	StatusBar statusBar(canvasRef, coloringRef);
	StatusBar* statusBarRef = &statusBar;

	UiHandler uiHandler(canvasRef, coloringRef, statusBarRef);
	UiHandler* uiHandlerRef = &uiHandler;

	FileHandler fileHandler(canvasRef, coloringRef, statusBarRef ,uiHandlerRef);
	FileHandler* fileHandlerRef = &fileHandler;

	KeyHandler keyHandler(canvasRef, coloringRef, uiHandlerRef, fileHandlerRef);
	KeyHandler* keyHandlerRef = &keyHandler;

	while (!uiHandlerRef->getExitApplication()) {
		canvasRef->resizeCanvas();
		statusBarRef->updateStatusBar();
		refresh();
		int keyInput = getch();
		keyHandlerRef->handleKeyInput(keyInput);
	}

	clear();
	refresh();
	endwin();

	return 0;
}
