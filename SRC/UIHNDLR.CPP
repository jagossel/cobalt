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
#include <string>
#include <CURSES.H>
#include "UIHNDLR.HPP"
#include "KEYDEF.HPP"
#include "DIALOGS.CPP"
#include "MACROS.HPP"

bool UiHandler::getConfirmation(std::string dialogMessage) {
	int dialogY = this->statusBar->getAtTop() ?
		this->canvas->getMinimumCursorY() :
		this->canvas->getMaximumCursorY();

	WINDOW* dialog = newwin(1, 80, dialogY, 0);
	wbkgdset(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(9, 0)));
	wattron(dialog, A_BOLD);
	wprintw(dialog, "%s (Y/N) ", dialogMessage.c_str());
	wrefresh(dialog);

	bool confirmationResult = false;
	bool exitDialog = false;
	while (!exitDialog) {
		int dialogKeyInput = wgetch(dialog);
		if ('Y' == dialogKeyInput || 'y' == dialogKeyInput) {
			confirmationResult = true;
			exitDialog = true;
		} else if ('N' == dialogKeyInput || 'n' == dialogKeyInput) {
			exitDialog = true;
		}
	}

	wclear(dialog);
	wrefresh(dialog);
	touchwin(stdscr);
	refresh();
	delwin(dialog);

	return confirmationResult;
}

bool UiHandler::getExitApplication() {
	return this->exitApplication;
}

void UiHandler::toggleExitApplication() {
	this->exitApplication = true;
}

void UiHandler::askCharacterSet() {
	int minimumCharacterSetIndex = this->canvas->getMinimumCharacterSetIndex();
	int maximumCharacterSetIndex = this->canvas->getMaximumCharacterSetIndex();
	int characterSetCount = maximumCharacterSetIndex - minimumCharacterSetIndex + 1;
	int windowHeight = characterSetCount + 2;

	WINDOW* dialog = Dialogs::createDialog(this->canvas, 27, windowHeight);
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

	Dialogs::destroyDialog(dialog);
}

void UiHandler::askColors() {
	int minimumBackgroundColor = this->coloring->getMinimumBackgroundColor();
	int maximumBackgroundColor = this->coloring->getMaximumBackgroundColor();
	int colorSwatchWidth = (maximumBackgroundColor - minimumBackgroundColor + 1) * 3;
	int windowWidth = colorSwatchWidth + 5;

	int minimumForegroundColor = this->coloring->getMinimumForegroundColor();
	int maximumForegroundColor = this->coloring->getMaximumForegroundColor();
	int colorSwatchHeight = maximumForegroundColor - minimumForegroundColor + 1;
	int windowHeight = colorSwatchHeight + 3;

	int selectedForegroundColor = this->coloring->getCurrentForegroundColor();
	int selectedBackgroundColor = this->coloring->getCurrentBackgroundColor();

	WINDOW* dialog = Dialogs::createDialog(this->canvas, windowWidth, windowHeight);
	auto drawColorSwatch = [&] ( ) {
		for (int drawRow = 0; drawRow < colorSwatchHeight; drawRow++) {
			int y = drawRow + 1;
			for (int drawCol = 0; drawCol < colorSwatchWidth; drawCol++) {
				int x = drawCol + 4;
				wmove(dialog, y, x);
				wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(selectedForegroundColor, selectedBackgroundColor)));
				if (selectedForegroundColor > 7) {
					wattron(dialog, A_BOLD);
				} else {
					wattroff(dialog, A_BOLD);
				}

				if (selectedBackgroundColor > 7) {
					wattron(dialog, A_BLINK);
				} else {
					wattroff(dialog, A_BLINK);
				}

				wprintw(dialog, "%c", 0xC5);
			}
		}
	};

	auto toggleForegroundColorHighlight = [&] (int selectedIndex, bool highlight) {
		int itemForegroundColor = highlight ? 0 : 7;
		int itemBackgroundColor = highlight ? 7 : 0;
		int y = selectedIndex + 1;
		wmove(dialog, y, 1);
		wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(itemForegroundColor, itemBackgroundColor)));
		wattroff(dialog, A_BLINK);
		wattroff(dialog, A_BOLD);
		wprintw(dialog, "% 3d", selectedIndex);
	};

	auto toggleBackgroundColorHighlight = [&] (int selectedIndex, bool highlight) {
		int itemForegroundColor = highlight ? 0 : 7;
		int itemBackgroundColor = highlight ? 7 : 0;
		int y = windowHeight - 2;
		int x = (selectedIndex * 3) + 4;
		wmove(dialog, y, x);
		wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(itemForegroundColor, itemBackgroundColor)));
		wattroff(dialog, A_BOLD);
		wattroff(dialog, A_BLINK);
		wprintw(dialog, "% 3d", selectedIndex);
	};

	for (int numberRow = minimumForegroundColor; numberRow <= maximumForegroundColor; numberRow++) {
		toggleForegroundColorHighlight(numberRow, numberRow == selectedForegroundColor);
	}

	for (int numberCol = minimumBackgroundColor; numberCol <= maximumBackgroundColor; numberCol++) {
		toggleBackgroundColorHighlight(numberCol, numberCol == selectedBackgroundColor);
	}

	drawColorSwatch();

	bool exitDialog = false;
	while (!exitDialog) {
		int keyInput = wgetch(dialog);
		if (KEY_UP == keyInput) {
			if (selectedForegroundColor > minimumForegroundColor) {
				toggleForegroundColorHighlight(selectedForegroundColor, false);
				selectedForegroundColor--;
				toggleForegroundColorHighlight(selectedForegroundColor, true);
				drawColorSwatch();
			}
		} else if (KEY_DOWN == keyInput) {
			if (selectedForegroundColor < maximumForegroundColor) {
				toggleForegroundColorHighlight(selectedForegroundColor, false);
				selectedForegroundColor++;
				toggleForegroundColorHighlight(selectedForegroundColor, true);
				drawColorSwatch();
			}
		} else if (KEY_LEFT == keyInput) {
			if (selectedBackgroundColor > minimumBackgroundColor) {
				toggleBackgroundColorHighlight(selectedBackgroundColor, false);
				selectedBackgroundColor--;
				toggleBackgroundColorHighlight(selectedBackgroundColor, true);
				drawColorSwatch();
			}
		} else if (KEY_RIGHT == keyInput) {
			if (selectedBackgroundColor < maximumBackgroundColor) {
				toggleBackgroundColorHighlight(selectedBackgroundColor, false);
				selectedBackgroundColor++;
				toggleBackgroundColorHighlight(selectedBackgroundColor, true);
				drawColorSwatch();
			}
		} else if (COBALT_KEY_ENTER == keyInput) {
			this->coloring->setCurrentForegroundColor(selectedForegroundColor);
			this->coloring->setCurrentBackgroundColor(selectedBackgroundColor);
			exitDialog = true;
		} else if (COBALT_KEY_ESCAPE == keyInput) {
			exitDialog = true;
		}
	}

	Dialogs::destroyDialog(dialog);
}
