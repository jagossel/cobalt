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

using namespace std;

// There are some keys that are not working in PDCurses; here, we are going to define our own.
#define COBALT_KEY_CTL_F     0x06
#define COBALT_KEY_BACKSPACE 0x08
#define COBALT_KEY_ENTER     0x0D
#define COBALT_KEY_ESCAPE    0x1B

class DrawingCharacter {
	public:
		char character;
		int foregroundColor;
		int backgroundColor;
};

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

char characterSets[11][10] =
{
	{ '\xB0', '\xB1', '\xB2', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF', '\xFE', '\x20' },
	{ '\xDA', '\xBF', '\xC4', '\xC2', '\xC1', '\xC3', '\xB4', '\xB3', '\xC0', '\xD9' },
	{ '\xC9', '\xBB', '\xCD', '\xCB', '\xCA', '\xCC', '\xB9', '\xBA', '\xC8', '\xBC' },
	{ '\xD6', '\xB7', '\xC4', '\xD2', '\xD0', '\xC7', '\xB6', '\xBA', '\xD3', '\xBD' },
	{ '\xD5', '\xB8', '\xCD', '\xD1', '\xCF', '\xC6', '\xB5', '\xB3', '\xD4', '\xBE' },
	{ '\xC5', '\xCE', '\xD7', '\xD8', '\xF0', '\xF4', '\xF5', '\xAE', '\xAF', '\xFE' },
	{ '\x2E', '\xF9', '\xFA', '\xF8', '\x2A', '\x0F', '\xF0', '\xF1', '\xF2', '\xF3' },
	{ '\xF4', '\xF5', '\xF6', '\xF7', '\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD' },
	{ '\x1E', '\x1F', '\x10', '\x11', '\x12', '\x1D', '\x18', '\x19', '\x1A', '\x1B' },
	{ '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x0B', '\x0C', '\x0F', '\x20' }
};

// This is the same order as the ANSI display driver color list.
short colorList[8] = {
	COLOR_BLACK,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BLUE,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_WHITE
};

void redrawRegion(int startX, int startY, int endX, int endY);
void init();
void loadScreenSize();
void handleKeyInput(int keyInput);
void updateStatusBar();
void initDrawing();
void initColors();
void setColors(int foregroundColor, int backgroundColor);
void resetColors();
int getColorPairIndex(int foregroundColorIndex, int backgroundColorIndex);
void drawCharacterFromSet(int chracterIndex);
void drawTypedCharacter(char typedCharacter);
void advanceColumn(bool backwards);
void advanceRow(bool backwards);
void resetCharacter();
void showFullDrawing();

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
	curs_set(false);
	for (int redrawY = startY; redrawY <= endY; redrawY++) {
		for (int redrawX = startX; redrawX <= endX; redrawX++) {
			int yIndex = redrawY - minimumCursorY;
			int xIndex = redrawX - minimumCursorX;
			int foregroundColor = canvas[yIndex][xIndex].foregroundColor;
			int backgroundColor = canvas[yIndex][xIndex].backgroundColor;

			move(redrawY, redrawX);
			setColors(foregroundColor, backgroundColor);
			printw("%c", canvas[yIndex][xIndex].character);
		}
	}

	resetColors();
	move(currentCursorY, currentCursorX);
	curs_set(true);
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
			canvas[initY][initX].character = ' ';
			canvas[initY][initX].foregroundColor = 7;
			canvas[initY][initX].backgroundColor = 0;
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
	canvas[currentCursorY][currentCursorX].character = typedCharacter;
	canvas[currentCursorY][currentCursorX].foregroundColor = currentForegroundColor;
	canvas[currentCursorY][currentCursorX].backgroundColor = currentBackgroundColor;

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
	canvas[currentCursorY][currentCursorX].character = ' ';
	canvas[currentCursorY][currentCursorX].foregroundColor = 7;
	canvas[currentCursorY][currentCursorX].backgroundColor = 0;

	setColors(7, 0);
	printw("%c", ' ');
}

void showFullDrawing() {
	curs_set(0);

	int redrawRegionY = statusBarAtTop ? minimumCursorY : maximumCursorY;
	redrawRegion(minimumCursorX, redrawRegionY, maximumCursorX, redrawRegionY);
	getch();

	curs_set(1);
}
