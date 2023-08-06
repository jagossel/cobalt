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
