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
#include "DIALOGS.HPP"

WINDOW* Dialogs::createDialog(int windowWidth, int windowHeight) {
	int minimumCursorX = this->canvas->getMinimumCursorX();
	int maximumCursorX = this->canvas->getMaximumCursorX();
	int screenWidth = maximumCursorX - minimumCursorX + 1;

	int minimumCursorY = this->canvas->getMinimumCursorY();
	int maximumCursorY = this->canvas->getMaximumCursorY();
	int screenHeight = maximumCursorY - minimumCursorY;

	int startX = DIALOG_CENTER_START(screenWidth, windowWidth);
	int startY = DIALOG_CENTER_START(screenHeight, windowHeight);
	int endX = startX + windowWidth;
	int endY = startY + windowHeight;

	curs_set(0);
	WINDOW* dialog = newwin(windowHeight, windowWidth, startY, startX);
	wbkgdset(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(6, 0)));
	wborder(dialog, 0xB3, 0xB3, 0xCD, 0xCD, 0xD5, 0xB8, 0xD4, 0xBE);
	wrefresh(dialog);
	keypad(dialog, true);

	return dialog;
}

void Dialogs::destroyDialog(WINDOW* dialog) {
	wclear(dialog);
	wrefresh(dialog);
	touchwin(stdscr);
	refresh();
	delwin(dialog);
}
