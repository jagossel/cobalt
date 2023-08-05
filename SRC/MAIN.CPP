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
const int minimumForegroundColor = 0;
const int maximumForegroundColor = 15;
const int minimumBackgroundColor = 0;
const int maximumBackgroundColor = 15;

bool exitApplication = false;
bool statusBarAtTop = false;

int minimumCursorX = 0;
int minimumCursorY = 0;

int maximumCursorX = 0;
int maximumCursorY = 0;

int currentCursorX = 0;
int currentCursorY = 0;

int currentCharacterSetIndex = 0;
int currentForegroundColor = 7;
int currentBackgroundColor = 0;

DrawingCharacter canvas[25][80];

int main() {
	initscr();
	raw();
	raw_output(true);
	keypad(stdscr, true);
	noecho();
	start_color();

	init();
	initDrawing();
	initColors();
	redrawRegion(minimumCursorX, minimumCursorY, maximumCursorX, maximumCursorY);

	while (!exitApplication) {
		loadScreenSize();
		updateStatusBar();
		refresh();
		int keyInput = getch();
		handleKeyInput(keyInput);
	}

	clear();
	refresh();
	endwin();

	return 0;
}

void redrawRegion(int startX, int startY, int endX, int endY) {
	curs_set(0);
	for (int redrawY = startY; redrawY <= endY; redrawY++) {
		for (int redrawX = startX; redrawX <= endX; redrawX++) {
			int yIndex = redrawY - minimumCursorY;
			int xIndex = redrawX - minimumCursorX;
			int foregroundColor = canvas[yIndex][xIndex].getForegroundColor();
			int backgroundColor = canvas[yIndex][xIndex].getBackgroundColor();

			move(redrawY, redrawX);
			setColors(foregroundColor, backgroundColor);
			printw("%c", canvas[yIndex][xIndex].getCharacter());
		}
	}

	resetColors();
	move(currentCursorY, currentCursorX);
	curs_set(1);
}

void init() {
	loadScreenSize();
	currentCursorY = minimumCursorY;
	currentCursorX = minimumCursorX;
}

void loadScreenSize() {
	getbegyx(stdscr, minimumCursorY, minimumCursorX);
	getmaxyx(stdscr, maximumCursorY, maximumCursorX);

	// Not sure why this needs to be done, but the maximum range has to be decremented in order to keep the cursor
	// from sticking to the bottom of the screen.
	maximumCursorY--;
	maximumCursorX--;
}

void handleKeyInput(int keyInput) {
	if (KEY_UP == keyInput) {
		advanceRow(true);
	} else if (KEY_DOWN == keyInput) {
		advanceRow(false);
	} else if (KEY_LEFT == keyInput) {
		advanceColumn(true);
	} else if (KEY_RIGHT == keyInput) {
		advanceColumn(false);
	} else if (KEY_HOME == keyInput) {
		currentCursorX = minimumCursorX;
		move(currentCursorY, currentCursorX);
	} else if (KEY_END == keyInput) {
		currentCursorX = maximumCursorX;
		move(currentCursorY, currentCursorX);
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
		if (currentForegroundColor <= minimumForegroundColor) {
			currentForegroundColor = maximumForegroundColor;
		} else {
			currentForegroundColor--;
		}
	} else if (CTL_DOWN == keyInput) {
		if (currentForegroundColor >= maximumForegroundColor) {
			currentForegroundColor = minimumForegroundColor;
		} else {
			currentForegroundColor++;
		}
	} else if (CTL_LEFT == keyInput) {
		if (currentBackgroundColor <= minimumBackgroundColor) {
			currentBackgroundColor = maximumBackgroundColor;
		} else {
			currentBackgroundColor--;
		}
	} else if (CTL_RIGHT == keyInput) {
		if (currentBackgroundColor >= maximumBackgroundColor) {
			currentBackgroundColor = minimumBackgroundColor;
		} else {
			currentBackgroundColor++;
		}
	} else if (keyInput >= KEY_F(1) && keyInput <= KEY_F(10)) {
		drawCharacterFromSet(keyInput - KEY_F0 - 1);
	} else if (keyInput >= 32 && keyInput <= 126) {
		drawTypedCharacter(keyInput);
	} else if (COBALT_KEY_BACKSPACE == keyInput) {
		advanceColumn(true);
		resetCharacter();
	} else if (KEY_DC == keyInput) {
		resetCharacter();
	} else if (COBALT_KEY_ENTER == keyInput) {
		if (currentCursorY >= maximumCursorY) return;
		advanceRow(false);
		currentCursorX = minimumCursorX;
	} else if (COBALT_KEY_CTL_F == keyInput) {
		showFullDrawing();
	} else if (COBALT_KEY_CTL_N == keyInput) {
		newDrawing();
	} else if (COBALT_KEY_ESCAPE == keyInput) {
		exitApplication = true;
	}
}

void updateStatusBar() {
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
			redrawRegion(minimumCursorX, maximumCursorY,
				maximumCursorX, maximumCursorY);
		} else {
			redrawRegion(minimumCursorX, minimumCursorY,
				maximumCursorX, minimumCursorY);
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
			canvas[initY][initX].setCharacter(' ');
			canvas[initY][initX].setForegroundColor(7);
			canvas[initY][initX].setBackgroundColor(0);
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

void drawCharacterFromSet(int chracterIndex) {
	drawTypedCharacter(characterSets[currentCharacterSetIndex][chracterIndex]);
}

void drawTypedCharacter(char typedCharacter) {
	canvas[currentCursorY][currentCursorX].setCharacter(typedCharacter);
	canvas[currentCursorY][currentCursorX].setForegroundColor(currentForegroundColor);
	canvas[currentCursorY][currentCursorX].setBackgroundColor(currentBackgroundColor);

	setColors(currentForegroundColor, currentBackgroundColor);
	printw("%c", typedCharacter);
	resetColors();
	advanceColumn(false);
}

void advanceColumn(bool backwards) {
	if (backwards) {
		if (currentCursorX <= minimumCursorX) return;
		currentCursorX--;
	} else {
		if (currentCursorX >= maximumCursorX) return;
		currentCursorX++;
	}

	move(currentCursorY, currentCursorX);
}

void advanceRow(bool backwards) {
	if (backwards) {
		if (currentCursorY <= minimumCursorY) return;
		currentCursorY--;
	} else {
		if (currentCursorY >= maximumCursorY) return;
		currentCursorY++;
	}

	move(currentCursorY, currentCursorX);
}

void resetCharacter() {
	canvas[currentCursorY][currentCursorX].setCharacter(' ');
	canvas[currentCursorY][currentCursorX].setForegroundColor(7);
	canvas[currentCursorY][currentCursorX].setBackgroundColor(0);

	setColors(7, 0);
	printw("%c", ' ');
}

void showFullDrawing() {
	int redrawRegionY = statusBarAtTop ? minimumCursorY : maximumCursorY;
	redrawRegion(minimumCursorX, redrawRegionY, maximumCursorX, redrawRegionY);

	curs_set(0);
	getch();
	curs_set(1);
}

bool getConfirmation(const char* dialogMessage) {
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

void newDrawing() {
	if (getConfirmation("Start a new Drawing?")) {
		currentCursorX = 0;
		currentCursorY = 0;
		currentCharacterSetIndex = 0;
		currentForegroundColor = 7;
		currentBackgroundColor = 0;

		move(currentCursorX, currentCursorY);
		initDrawing();
	}

	redrawRegion(minimumCursorX, minimumCursorY, maximumCursorX, maximumCursorY);
}
