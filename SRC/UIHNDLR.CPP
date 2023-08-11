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
#include <CURSES.H>
#include "UIHNDLR.HPP"
#include "KEYDEF.HPP"

bool UiHandler::getConfirmation(const char* dialogMessage) {
	int minimumCursorX = this->canvas->getMinimumCursorY();
	int minimumCursorY = this->canvas->getMinimumCursorY();
	int maximumCursorY = this->canvas->getMaximumCursorY();

	int dialogY = this->statusBar->getAtTop() ? minimumCursorY : maximumCursorY;

	move(dialogY, minimumCursorX);
	this->coloring->setColors(7, 0);
	printw("% 80c", ' ');

	move(dialogY, minimumCursorX);
	this->coloring->setColors(9, 0);
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

bool UiHandler::getExitApplication() {
	return this->exitApplication;
}

void UiHandler::toggleExitApplication() {
	this->exitApplication = true;
}

void UiHandler::askCharacterSet() {
	int minimumCursorX = this->canvas->getMinimumCursorX();
	int maximumCursorX = this->canvas->getMaximumCursorX();
	int screenWidth = maximumCursorX - minimumCursorX + 1;

	int minimumCursorY = this->canvas->getMinimumCursorY();
	int maximumCursorY = this->canvas->getMaximumCursorY();
	int screenHeight = maximumCursorY - minimumCursorY;

	int windowWidth = 27;

	int minimumCharacterSetIndex = this->canvas->getMinimumCharacterSetIndex();
	int maximumCharacterSetIndex = this->canvas->getMaximumCharacterSetIndex();
	int characterSetCount = maximumCharacterSetIndex - minimumCharacterSetIndex + 1;
	int windowHeight = characterSetCount + 2;

	int startX = CENTER_START(screenWidth, windowWidth);
	int startY = CENTER_START(screenHeight, windowHeight);
	int endX = startX + windowWidth;
	int endY = startY + windowHeight;

	curs_set(0);
	WINDOW* dialog = newwin(windowHeight, windowWidth, startY, startX);
	wbkgdset(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(6, 0)));
	wborder(dialog, 0xB3, 0xB3, 0xCD, 0xCD, 0xD5, 0xB8, 0xD4, 0xBE);
	wrefresh(dialog);
	keypad(dialog, true);

	auto printSelectedCharacterSet = [&] (WINDOW* window, int selectedIndex, bool highlight) {
		int foregroundColor = highlight ? 0 : 7;
		int backgroundColor = highlight ? 7 : 0;
		int linePosition = selectedIndex + 1;

		wmove(window, linePosition, 1);
		wattron(window, COLOR_PAIR(COLOR_PAIR_INDEX(foregroundColor, backgroundColor)));
		wprintw(
			window,
			"% 3d: %c %c %c %c %c %c %c %c %c %c ",
			linePosition,
			this->canvas->getCharacterFromSet(selectedIndex, 0),
			this->canvas->getCharacterFromSet(selectedIndex, 1),
			this->canvas->getCharacterFromSet(selectedIndex, 2),
			this->canvas->getCharacterFromSet(selectedIndex, 3),
			this->canvas->getCharacterFromSet(selectedIndex, 4),
			this->canvas->getCharacterFromSet(selectedIndex, 5),
			this->canvas->getCharacterFromSet(selectedIndex, 6),
			this->canvas->getCharacterFromSet(selectedIndex, 7),
			this->canvas->getCharacterFromSet(selectedIndex, 8),
			this->canvas->getCharacterFromSet(selectedIndex, 9));
	};

	int selectedCharacterSetIndex = this->canvas->getCurrentCharacterSetIndex();
	for (int drawLine = 0; drawLine <= maximumCharacterSetIndex; drawLine++) {
		int linePosition = drawLine + 1;
		printSelectedCharacterSet(dialog, drawLine, drawLine == selectedCharacterSetIndex);
	}

	bool closeDialog = false;
	while (!closeDialog) {
		int keyInput = wgetch(dialog);
		if (KEY_UP == keyInput) {
			if (selectedCharacterSetIndex > minimumCharacterSetIndex) {
				printSelectedCharacterSet(dialog, selectedCharacterSetIndex, false);
				selectedCharacterSetIndex--;
				printSelectedCharacterSet(dialog, selectedCharacterSetIndex, true);
				wrefresh(dialog);
			}
		} else if (KEY_DOWN == keyInput) {
			if (selectedCharacterSetIndex < maximumCharacterSetIndex) {
				printSelectedCharacterSet(dialog, selectedCharacterSetIndex, false);
				selectedCharacterSetIndex++;
				printSelectedCharacterSet(dialog, selectedCharacterSetIndex, true);
				wrefresh(dialog);
			}
		} else if (COBALT_KEY_ESCAPE == keyInput) {
			closeDialog = true;
		} else if (COBALT_KEY_ENTER == keyInput) {
			closeDialog = true;
			this->canvas->setCurrentCharacterSetIndex(selectedCharacterSetIndex);
		}
	}

	wclear(dialog);
	wrefresh(dialog);
	touchwin(stdscr);
	refresh();

	delwin(dialog);
}
