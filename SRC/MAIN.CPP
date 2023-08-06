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

#include "GLOBAL.H"
#include "KEYDEF.H"
#include "CHARSETS.H"
#include "FUNCS.H"
#include "FILEIO.H"
#include "UI.H"
#include "DRAWING.H"

using namespace std;

const int statusBarYTolerance = 2;
const int minimumCharacterSetIndex = 0;
const int maximumCharacterSetIndex = 9;

bool exitApplication = false;
bool statusBarAtTop = false;

int currentCharacterSetIndex = 0;

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

	initDrawing();
	initColors();
	redrawRegion(
		canvasRef,
		canvasRef->getMinimumCursorX(),
		canvasRef->getMinimumCursorY(),
		canvasRef->getMaximumCursorX(),
		canvasRef->getMaximumCursorY());

	while (!exitApplication) {
		canvasRef->resizeCanvas();
		updateStatusBar(canvasRef);
		refresh();
		int keyInput = getch();
		handleKeyInput(canvasRef, keyInput);
	}

	clear();
	refresh();
	endwin();

	return 0;
}

void redrawRegion(Canvas* canvas, int startX, int startY, int endX, int endY) {
	curs_set(0);
	for (int redrawY = startY; redrawY <= endY; redrawY++) {
		for (int redrawX = startX; redrawX <= endX; redrawX++) {
			int yIndex = redrawY - canvas->getMinimumCursorY();
			int xIndex = redrawX - canvas->getMinimumCursorX();
			int foregroundColor = drawingCanvas[yIndex][xIndex].getForegroundColor();
			int backgroundColor = drawingCanvas[yIndex][xIndex].getBackgroundColor();

			move(redrawY, redrawX);
			setColors(foregroundColor, backgroundColor);
			printw("%c", drawingCanvas[yIndex][xIndex].getCharacter());
		}
	}

	resetColors();
	move(canvas->getCurrentCursorY(), canvas->getCurrentCursorX());
	curs_set(1);
}

void handleKeyInput(Canvas* canvas, int keyInput) {
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
		if (currentCharacterSetIndex >= maximumCharacterSetIndex) {
			currentCharacterSetIndex = minimumCharacterSetIndex;
		} else {
			currentCharacterSetIndex++;
		}
	} else if (ALT_DOWN == keyInput) {
		if (currentCharacterSetIndex <= minimumCharacterSetIndex) {
			currentCharacterSetIndex = maximumCharacterSetIndex;
		} else {
			currentCharacterSetIndex--;
		}
	} else if (CTL_UP == keyInput) {
		canvas->previousForegroundColor();
	} else if (CTL_DOWN == keyInput) {
		canvas->nextForegroundColor();
	} else if (CTL_LEFT == keyInput) {
		canvas->previousBackgroundColor();
	} else if (CTL_RIGHT == keyInput) {
		canvas->nextBackgroundColor();
	} else if (keyInput >= KEY_F(1) && keyInput <= KEY_F(10)) {
		drawCharacterFromSet(canvas, keyInput - KEY_F0 - 1);
	} else if (keyInput >= 32 && keyInput <= 126) {
		drawTypedCharacter(canvas, keyInput);
	} else if (COBALT_KEY_BACKSPACE == keyInput) {
		canvas->moveLeft();
		resetCharacter(canvas);
	} else if (KEY_DC == keyInput) {
		resetCharacter(canvas);
	} else if (COBALT_KEY_ENTER == keyInput) {
		canvas->moveToNextLine();
	} else if (COBALT_KEY_CTL_F == keyInput) {
		showFullDrawing(canvas);
	} else if (COBALT_KEY_CTL_N == keyInput) {
		newDrawing(canvas);
	} else if (COBALT_KEY_ESCAPE == keyInput) {
		exitApplication = true;
	}
}

void updateStatusBar(Canvas* canvas) {
	int minimumCursorY = canvas->getMinimumCursorY();
	int currentCursorY = canvas->getCurrentCursorY();
	int maximumCursorY = canvas->getMaximumCursorY();

	int minimumCursorX = canvas->getMinimumCursorX();
	int currentCursorX = canvas->getCurrentCursorX();
	int maximumCursorX = canvas->getMaximumCursorX();

	int currentForegroundColor = canvas->getCurrentForegroundColor();
	int currentBackgroundColor = canvas->getCurrentBackgroundColor();

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
				minimumCursorX,
				maximumCursorY,
				maximumCursorX,
				maximumCursorY);
		} else {
			redrawRegion(
				canvas,
				minimumCursorX,
				minimumCursorY,
				maximumCursorX,
				minimumCursorY);
		}
	}

	curs_set(0);
	setColors(currentForegroundColor, currentBackgroundColor);
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
		characterSets[currentCharacterSetIndex][0],
		characterSets[currentCharacterSetIndex][1],
		characterSets[currentCharacterSetIndex][2],
		characterSets[currentCharacterSetIndex][3],
		characterSets[currentCharacterSetIndex][4],
		characterSets[currentCharacterSetIndex][5],
		characterSets[currentCharacterSetIndex][6],
		characterSets[currentCharacterSetIndex][7],
		characterSets[currentCharacterSetIndex][8],
		characterSets[currentCharacterSetIndex][9]);

	resetColors();
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

void initColors() {
	for (int initBackgroundColor = 0; initBackgroundColor < 16; initBackgroundColor++) {
		int backgroundColorIndex = initBackgroundColor % 8;
		short backgroundColor = colorList[backgroundColorIndex];
		for (int initForegroundColor = 0; initForegroundColor < 16; initForegroundColor++) {
			int foregroundColorIndex = initForegroundColor % 8;
			int colorPairIndex = getColorPairIndex(initForegroundColor, initBackgroundColor);
			short foregroundColor = colorList[foregroundColorIndex];

			init_pair(colorPairIndex, foregroundColor, backgroundColor);
		}
	}
}

void setColors(int foregroundColor, int backgroundColor) {
	int colorPairIndex = getColorPairIndex(foregroundColor, backgroundColor);
	attron(COLOR_PAIR(colorPairIndex));
	if (foregroundColor > 7) {
		attron(A_BOLD);
	} else {
		attroff(A_BOLD);
	}

	if (backgroundColor > 7) {
		attron(A_BLINK);
	} else {
		attroff(A_BLINK);
	}
}

void resetColors() {
	int colorPairIndex = getColorPairIndex(7, 0);
	attroff(COLOR_PAIR(colorPairIndex));
	attroff(A_BOLD);
	attroff(A_BLINK);
}

int getColorPairIndex(int foregroundColorIndex, int backgroundColorIndex) {
	return (backgroundColorIndex * 16) + foregroundColorIndex;
}

void drawCharacterFromSet(Canvas* canvas, int chracterIndex) {
	drawTypedCharacter(canvas, characterSets[currentCharacterSetIndex][chracterIndex]);
}

void drawTypedCharacter(Canvas* canvas, char typedCharacter) {
	int currentCursorX = canvas->getCurrentCursorX();
	int currentCursorY = canvas->getCurrentCursorY();
	int currentForegroundColor = canvas->getCurrentForegroundColor();
	int currentBackgroundColor = canvas->getCurrentBackgroundColor();

	drawingCanvas[currentCursorY][currentCursorX].setCharacter(typedCharacter);
	drawingCanvas[currentCursorY][currentCursorX].setForegroundColor(currentForegroundColor);
	drawingCanvas[currentCursorY][currentCursorX].setBackgroundColor(currentBackgroundColor);

	setColors(currentForegroundColor, currentBackgroundColor);
	printw("%c", typedCharacter);
	resetColors();
	canvas->moveRight();
}

void resetCharacter(Canvas* canvas) {
	int currentCursorX = canvas->getCurrentCursorX();
	int currentCursorY = canvas->getCurrentCursorY();
	drawingCanvas[currentCursorY][currentCursorX].setCharacter(' ');
	drawingCanvas[currentCursorY][currentCursorX].setForegroundColor(7);
	drawingCanvas[currentCursorY][currentCursorX].setBackgroundColor(0);

	setColors(7, 0);
	printw("%c", ' ');
}

void showFullDrawing(Canvas* canvas) {
	int minimumCursorX = canvas->getMinimumCursorX();
	int maximumCursorX = canvas->getMaximumCursorX();
	int minimumCursorY = canvas->getMinimumCursorY();
	int maximumCursorY = canvas->getMaximumCursorY();

	int redrawRegionY = statusBarAtTop ? minimumCursorY : maximumCursorY;
	redrawRegion(canvas, minimumCursorX, redrawRegionY, maximumCursorX, redrawRegionY);

	curs_set(0);
	getch();
	curs_set(1);
}

bool getConfirmation(Canvas* canvas, const char* dialogMessage) {
	int minimumCursorX = canvas->getMinimumCursorY();
	int minimumCursorY = canvas->getMinimumCursorY();
	int maximumCursorY = canvas->getMaximumCursorY();

	int dialogY = statusBarAtTop ? minimumCursorY : maximumCursorY;

	move(dialogY, minimumCursorX);
	setColors(7, 0);
	printw("% 80c", ' ');

	move(dialogY, minimumCursorX);
	setColors(9, 0);
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

void newDrawing(Canvas* canvas) {
	if (getConfirmation(canvas, "Start a new Drawing?")) {
		canvas->reset();
		currentCharacterSetIndex = 0;

		move(canvas->getCurrentCursorX(), canvas->getCurrentCursorY());
		initDrawing();
	}

	redrawRegion(
		canvas,
		canvas->getMinimumCursorX(),
		canvas->getMinimumCursorY(),
		canvas->getMaximumCursorX(),
		canvas->getMaximumCursorY());
}
