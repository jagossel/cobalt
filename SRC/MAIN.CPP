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
#include "DRAWCHAR.CPP"
#include "CANVAS.CPP"
#include "COLORING.CPP"

#include "KEYDEF.H"
#include "FUNCS.H"
#include "FILEIO.H"
#include "UI.H"
#include "DRAWING.H"

using namespace std;

const int statusBarYTolerance = 2;

bool exitApplication = false;
bool statusBarAtTop = false;

DrawingCharacter drawingCanvas[25][80];

int main() {
	initscr();
	raw();
	raw_output(true);
	keypad(stdscr, true);
	noecho();
	start_color();

	Canvas canvas;
	Canvas* canvasRef = &canvas;
	canvasRef->resizeCanvas();

	Coloring coloring;
	Coloring* coloringRef = &coloring;
	coloringRef->initColors();

	initDrawing();
	redrawRegion(
		canvasRef,
		coloringRef,
		canvasRef->getMinimumCursorX(),
		canvasRef->getMinimumCursorY(),
		canvasRef->getMaximumCursorX(),
		canvasRef->getMaximumCursorY());

	while (!exitApplication) {
		canvasRef->resizeCanvas();
		updateStatusBar(canvasRef, coloringRef);
		refresh();
		int keyInput = getch();
		handleKeyInput(canvasRef, coloringRef, keyInput);
	}

	clear();
	refresh();
	endwin();

	return 0;
}

void redrawRegion(Canvas* canvas, Coloring* coloring, int startX, int startY, int endX, int endY) {
	curs_set(0);
	for (int redrawY = startY; redrawY <= endY; redrawY++) {
		for (int redrawX = startX; redrawX <= endX; redrawX++) {
			int yIndex = redrawY - canvas->getMinimumCursorY();
			int xIndex = redrawX - canvas->getMinimumCursorX();
			int foregroundColor = drawingCanvas[yIndex][xIndex].getForegroundColor();
			int backgroundColor = drawingCanvas[yIndex][xIndex].getBackgroundColor();

			move(redrawY, redrawX);
			coloring->setColors(foregroundColor, backgroundColor);
			printw("%c", drawingCanvas[yIndex][xIndex].getCharacter());
		}
	}

	coloring->resetColors();
	move(canvas->getCurrentCursorY(), canvas->getCurrentCursorX());
	curs_set(1);
}

void handleKeyInput(Canvas* canvas, Coloring* coloring, int keyInput) {
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
		drawCharacterFromSet(canvas, coloring, keyInput - KEY_F0 - 1);
	} else if (keyInput >= 32 && keyInput <= 126) {
		drawTypedCharacter(canvas, coloring, keyInput);
	} else if (COBALT_KEY_BACKSPACE == keyInput) {
		canvas->moveLeft();
		resetCharacter(canvas, coloring);
	} else if (KEY_DC == keyInput) {
		resetCharacter(canvas, coloring);
	} else if (COBALT_KEY_ENTER == keyInput) {
		canvas->moveToNextLine();
	} else if (COBALT_KEY_CTL_F == keyInput) {
		showFullDrawing(canvas, coloring);
	} else if (COBALT_KEY_CTL_N == keyInput) {
		newDrawing(canvas, coloring);
	} else if (COBALT_KEY_ESCAPE == keyInput) {
		exitApplication = true;
	}
}

void updateStatusBar(Canvas* canvas, Coloring* coloring) {
	int minimumCursorY = canvas->getMinimumCursorY();
	int currentCursorY = canvas->getCurrentCursorY();
	int maximumCursorY = canvas->getMaximumCursorY();

	int minimumCursorX = canvas->getMinimumCursorX();
	int currentCursorX = canvas->getCurrentCursorX();
	int maximumCursorX = canvas->getMaximumCursorX();

	int currentForegroundColor = coloring->getCurrentForegroundColor();
	int currentBackgroundColor = coloring->getCurrentBackgroundColor();

	int toggleStatusBarMoveY = statusBarAtTop ?
		minimumCursorY + statusBarYTolerance :
		maximumCursorY - statusBarYTolerance;

	bool moveStatusBar = false;
	if (statusBarAtTop) {
		if (currentCursorY <= toggleStatusBarMoveY) {
			statusBarAtTop = false;
			moveStatusBar = true;
		}
	} else {
		if (currentCursorY >= toggleStatusBarMoveY) {
			statusBarAtTop = true;
			moveStatusBar = true;
		}
	}

	int statusBarY = statusBarAtTop ? minimumCursorY : maximumCursorY;
	if (moveStatusBar) {
		if (statusBarAtTop) {
			redrawRegion(
				canvas,
				coloring,
				minimumCursorX,
				maximumCursorY,
				maximumCursorX,
				maximumCursorY);
		} else {
			redrawRegion(
				canvas,
				coloring,
				minimumCursorX,
				minimumCursorY,
				maximumCursorX,
				minimumCursorY);
		}
	}

	curs_set(0);
	coloring->applyColoring();
	move(statusBarY, minimumCursorX);
	printw("% 80c", ' ');

	move(statusBarY, minimumCursorX);

	int printCurrentCursorX = currentCursorX + 1;
	int printCurrentCursorY = currentCursorY + 1;
	printw(
		"%02d,%02d%c%02d,%02d%cF1=%c F2=%c F3=%c F4=%c F5=%c F6=%c F7=%c F8=%c F9=%c F10=%c",
		currentForegroundColor,
		currentBackgroundColor,
		'\xB3',
		printCurrentCursorX,
		printCurrentCursorY,
		'\xB3',
		canvas->getCharacterFromSet(0),
		canvas->getCharacterFromSet(1),
		canvas->getCharacterFromSet(2),
		canvas->getCharacterFromSet(3),
		canvas->getCharacterFromSet(4),
		canvas->getCharacterFromSet(5),
		canvas->getCharacterFromSet(6),
		canvas->getCharacterFromSet(7),
		canvas->getCharacterFromSet(8),
		canvas->getCharacterFromSet(9));

	coloring->resetColors();
	curs_set(1);
	move(currentCursorY, currentCursorX);
}

void initDrawing() {
	for (int initY = 0; initY < 25; initY++) {
		for (int initX = 0; initX < 80; initX++) {
			drawingCanvas[initY][initX].setCharacter(' ');
			drawingCanvas[initY][initX].setForegroundColor(7);
			drawingCanvas[initY][initX].setBackgroundColor(0);
		}
	}
}

void drawCharacterFromSet(Canvas* canvas, Coloring* coloring, int characterIndex) {
	drawTypedCharacter(canvas, coloring, canvas->getCharacterFromSet(characterIndex));
}

void drawTypedCharacter(Canvas* canvas, Coloring* coloring, char typedCharacter) {
	int currentCursorX = canvas->getCurrentCursorX();
	int currentCursorY = canvas->getCurrentCursorY();
	int currentForegroundColor = coloring->getCurrentForegroundColor();
	int currentBackgroundColor = coloring->getCurrentBackgroundColor();

	drawingCanvas[currentCursorY][currentCursorX].setCharacter(typedCharacter);
	drawingCanvas[currentCursorY][currentCursorX].setForegroundColor(currentForegroundColor);
	drawingCanvas[currentCursorY][currentCursorX].setBackgroundColor(currentBackgroundColor);

	coloring->applyColoring();
	printw("%c", typedCharacter);
	coloring->resetColors();
	canvas->moveRight();
}

void resetCharacter(Canvas* canvas, Coloring* coloring) {
	int currentCursorX = canvas->getCurrentCursorX();
	int currentCursorY = canvas->getCurrentCursorY();
	drawingCanvas[currentCursorY][currentCursorX].setCharacter(' ');
	drawingCanvas[currentCursorY][currentCursorX].setForegroundColor(7);
	drawingCanvas[currentCursorY][currentCursorX].setBackgroundColor(0);

	coloring->setColors(7, 0);
	printw("%c", ' ');
}

void showFullDrawing(Canvas* canvas, Coloring* coloring) {
	int minimumCursorX = canvas->getMinimumCursorX();
	int maximumCursorX = canvas->getMaximumCursorX();
	int minimumCursorY = canvas->getMinimumCursorY();
	int maximumCursorY = canvas->getMaximumCursorY();

	int redrawRegionY = statusBarAtTop ? minimumCursorY : maximumCursorY;
	redrawRegion(canvas, coloring, minimumCursorX, redrawRegionY, maximumCursorX, redrawRegionY);

	curs_set(0);
	getch();
	curs_set(1);
}

bool getConfirmation(Canvas* canvas, Coloring* coloring, const char* dialogMessage) {
	int minimumCursorX = canvas->getMinimumCursorY();
	int minimumCursorY = canvas->getMinimumCursorY();
	int maximumCursorY = canvas->getMaximumCursorY();

	int dialogY = statusBarAtTop ? minimumCursorY : maximumCursorY;

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

void newDrawing(Canvas* canvas, Coloring* coloring) {
	if (getConfirmation(canvas, coloring, "Start a new Drawing?")) {
		canvas->reset();
		coloring->reset();

		move(canvas->getCurrentCursorX(), canvas->getCurrentCursorY());
		initDrawing();
	}

	redrawRegion(
		canvas,
		coloring,
		canvas->getMinimumCursorX(),
		canvas->getMinimumCursorY(),
		canvas->getMaximumCursorX(),
		canvas->getMaximumCursorY());
}
