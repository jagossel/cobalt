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

#include "KEYDEF.H"
#include "FUNCS.H"
#include "FILEIO.H"
#include "UI.H"

using namespace std;

bool exitApplication = false;

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

	while (!exitApplication) {
		canvasRef->resizeCanvas();
		statusBarRef->updateStatusBar();
		refresh();
		int keyInput = getch();
		handleKeyInput(canvasRef, coloringRef, statusBarRef, keyInput);
	}

	clear();
	refresh();
	endwin();

	return 0;
}

void handleKeyInput(Canvas* canvas, Coloring* coloring, StatusBar* statusBar, int keyInput) {
	if (KEY_UP == keyInput) {
		canvas->moveUp();
	} else if (KEY_DOWN == keyInput) {
		canvas->moveDown();
	} else if (KEY_LEFT == keyInput) {
		canvas->moveLeft();
	} else if (KEY_RIGHT == keyInput) {
		canvas->moveRight();
	} else if (KEY_HOME == keyInput) {
		canvas->moveToHome();
	} else if (KEY_END == keyInput) {
		canvas->moveToEnd();
	} else if (ALT_UP == keyInput) {
		canvas->nextCharacterSet();
	} else if (ALT_DOWN == keyInput) {
		canvas->previousCharacterSet();
	} else if (CTL_UP == keyInput) {
		coloring->previousForegroundColor();
	} else if (CTL_DOWN == keyInput) {
		coloring->nextForegroundColor();
	} else if (CTL_LEFT == keyInput) {
		coloring->previousBackgroundColor();
	} else if (CTL_RIGHT == keyInput) {
		coloring->nextBackgroundColor();
	} else if (keyInput >= KEY_F(1) && keyInput <= KEY_F(10)) {
		canvas->drawCharacterFromSet(keyInput - KEY_F0 - 1);
	} else if (keyInput >= 32 && keyInput <= 126) {
		canvas->drawTypedCharacter(keyInput);
	} else if (COBALT_KEY_BACKSPACE == keyInput) {
		canvas->moveLeft();
		canvas->resetCharacter();
	} else if (KEY_DC == keyInput) {
		canvas->resetCharacter();
	} else if (COBALT_KEY_ENTER == keyInput) {
		canvas->moveToNextLine();
	} else if (COBALT_KEY_CTL_F == keyInput) {
		canvas->showFullDrawing();
	} else if (COBALT_KEY_CTL_N == keyInput) {
		newDrawing(canvas, coloring, statusBar);
	} else if (COBALT_KEY_ESCAPE == keyInput) {
		exitApplication = true;
	}
}

bool getConfirmation(Canvas* canvas, Coloring* coloring, StatusBar* statusBar, const char* dialogMessage) {
	int minimumCursorX = canvas->getMinimumCursorY();
	int minimumCursorY = canvas->getMinimumCursorY();
	int maximumCursorY = canvas->getMaximumCursorY();

	int dialogY = statusBar->getAtTop() ? minimumCursorY : maximumCursorY;

	move(dialogY, minimumCursorX);
	coloring->setColors(7, 0);
	printw("% 80c", ' ');

	move(dialogY, minimumCursorX);
	coloring->setColors(9, 0);
	printw("%s (Y/N) ", dialogMessage);

	bool confirmationResult = false;
	bool exitDialog = false;
	while (!exitDialog) {
		int dialogKeyInput = getch();
		if ('Y' == dialogKeyInput || 'y' == dialogKeyInput) {
			confirmationResult = true;
			exitDialog = true;
		} else if ('N' == dialogKeyInput || 'n' == dialogKeyInput) {
			exitDialog = true;
		}
	}

	return confirmationResult;
}

void newDrawing(Canvas* canvas, Coloring* coloring, StatusBar* statusBar) {
	if (!getConfirmation(canvas, coloring, statusBar, "Start a new Drawing?")) return;

	canvas->reset();
	coloring->reset();
	canvas->initializeDrawing();
	move(canvas->getCurrentCursorX(), canvas->getCurrentCursorY());
}
