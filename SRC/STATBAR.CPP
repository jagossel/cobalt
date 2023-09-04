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
#include "STATBAR.HPP"

bool StatusBar::getAtTop() {
	return this->atTop;
}

void StatusBar::updateStatusBar() {
	int minimumCursorY = this->canvas->getMinimumCursorY();
	int currentCursorY = this->canvas->getCurrentCursorY();
	int maximumCursorY = this->canvas->getMaximumCursorY();

	int minimumCursorX = this->canvas->getMinimumCursorX();
	int currentCursorX = this->canvas->getCurrentCursorX();
	int maximumCursorX = this->canvas->getMaximumCursorX();

	int currentForegroundColor = this->coloring->getCurrentForegroundColor();
	int currentBackgroundColor = this->coloring->getCurrentBackgroundColor();

	int toggleStatusBarMoveY = this->atTop ?
		minimumCursorY + this->statusBarYTolerance :
		maximumCursorY - this->statusBarYTolerance;

	bool moveStatusBar = false;
	if (this->atTop) {
		if (currentCursorY <= toggleStatusBarMoveY) {
			this->atTop = false;
			moveStatusBar = true;
		}
	} else {
		if (currentCursorY >= toggleStatusBarMoveY) {
			this->atTop = true;
			moveStatusBar = true;
		}
	}

	int statusBarY = this->atTop ? minimumCursorY : maximumCursorY;
	if (moveStatusBar) {
		if (this->atTop) {
			this->canvas->redrawRegion(minimumCursorX, maximumCursorY, maximumCursorX, maximumCursorY);
		} else {
			this->canvas->redrawRegion(minimumCursorX, minimumCursorY, maximumCursorX, minimumCursorY);
		}
	}

	curs_set(0);
	this->coloring->applyColoring();
	move(statusBarY, minimumCursorX);
	printw("% 80c", ' ');

	move(statusBarY, minimumCursorX);

	int printCurrentCursorX = currentCursorX + 1;
	int printCurrentCursorY = currentCursorY + 1;
	printw(
		"SHIFT+F1=HELP%c%02d,%02d%c%02d,%02d%cF1=%c F2=%c F3=%c F4=%c F5=%c F6=%c F7=%c F8=%c F9=%c F10=%c",
		'\xB3',
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

	this->coloring->resetColors();
	curs_set(1);
	move(currentCursorY, currentCursorX);
}
