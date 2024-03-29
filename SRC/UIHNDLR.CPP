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
#include <algorithm>
#include <string>
#include <vector>
#include <CURSES.H>
#include "UIHNDLR.HPP"
#include "KEYDEF.HPP"
#include "DIALOGS.CPP"
#include "MACROS.HPP"
#include "HELP.HPP"

std::string UiHandler::getFileName() {
	std::string dialogMessage = "Name w/o Extension: ";
	int dialogMessageLength = dialogMessage.length();
	int fullDialogMessageLength = dialogMessageLength + 8;
	int windowWidth = fullDialogMessageLength + 4;

	WINDOW* dialog = Dialogs::createDialog(this->canvas, windowWidth, 3);
	wmove(dialog, 1, 2);
	wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(7, 0)));
	wattroff(dialog, A_BOLD);
	wprintw(dialog, dialogMessage.c_str());

	std::string fileName = "";

	auto isFileNameCharacter = [&] (int keyInput) {
		return
			fileName.length() < 8
			&& (
				   (keyInput >= 0x30 && keyInput <= 0x39) // 0-9
				|| (keyInput >= 0x41 && keyInput <= 0x5A) // A-Z
				|| (keyInput >= 0x61 && keyInput <= 0x7A) // a-z
				|| keyInput == 0x5F // _
			);
	};

	auto displayFileName = [&] () {
		wmove(dialog, 1, dialogMessageLength + 2);
		wprintw(dialog, "% 8c", ' ');
		wmove(dialog, 1, dialogMessageLength + 2);
		wprintw(dialog, fileName.c_str());
	};

	bool exitDialog = false;
	while (!exitDialog) {
		curs_set(1);
		int dialogKeyInput = wgetch(dialog);
		if (isFileNameCharacter(dialogKeyInput)) {
			fileName += (char)(toupper(dialogKeyInput));
			displayFileName();
		} else if (COBALT_KEY_BACKSPACE == dialogKeyInput) {
			int fileNameLength = fileName.length();
			if (fileNameLength > 0) {
				int newFileNameLength = fileNameLength - 1;
				fileName = fileName.substr(0, newFileNameLength);
				displayFileName();
			}
		} else if (COBALT_KEY_ENTER == dialogKeyInput) {
			exitDialog = true;
		} else if (COBALT_KEY_ESCAPE == dialogKeyInput) {
			exitDialog = true;
			fileName = "";
		}
	}

	Dialogs::destroyDialog(dialog);

	return fileName;
}

bool UiHandler::getConfirmation(std::string dialogMessage) {
	std::string fullDialogMessage = std::string(dialogMessage) + " (Y/N)";
	int dialogMessageLenhth = fullDialogMessage.length();
	int windowWidth = dialogMessageLenhth + 4;

	WINDOW* dialog = Dialogs::createDialog(this->canvas, windowWidth, 3);
	wmove(dialog, 1, 2);
	wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(9, 0)));
	wattron(dialog, A_BOLD);
	wprintw(dialog, fullDialogMessage.c_str());

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

	Dialogs::destroyDialog(dialog);

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

char UiHandler::askUserInput(std::string dialogMessage, std::string acceptableKeys) {
	std::transform(acceptableKeys.begin(), acceptableKeys.end(), acceptableKeys.begin(), ::toupper);
	int dialogMessageLength = dialogMessage.length();
	int dialogWidth = dialogMessageLength + 4;

	WINDOW* dialog = Dialogs::createDialog(this->canvas, dialogWidth, 3);

	wmove(dialog, 1, 2);
	wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(7, 0)));
	wattroff(dialog, A_BOLD);
	wprintw(dialog, dialogMessage.c_str());

	unsigned char result = 0;
	bool exitDialog = false;
	while (!exitDialog) {
		int keyInput = wgetch(dialog);
		int keyInputUpper = toupper(keyInput);
		if (acceptableKeys.find_first_of(keyInputUpper) != std::string::npos) {
			exitDialog = true;
			result = keyInput;
		} else if (COBALT_KEY_ESCAPE == keyInput) {
			exitDialog = true;
		}
	}

	Dialogs::destroyDialog(dialog);

	return result;
}

void UiHandler::showErrorMessage(std::string errorMessage) {
	int errorMessageLength = errorMessage.length();
	int dialogWidth = errorMessageLength + 6;

	WINDOW* dialog = Dialogs::createDialog(this->canvas, dialogWidth, 5);

	wmove(dialog, 1, 2);
	wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(9, 0)));
	wattron(dialog, A_BOLD);
	wprintw(dialog, "!");

	wmove(dialog, 1, 4);
	wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(15, 0)));
	wattron(dialog, A_BOLD);
	wprintw(dialog, errorMessage.c_str());

	std::string pauseText = PAUSE_TEXT;
	int pauseTextLength = pauseText.length();
	int pauseTextX = DIALOG_CENTER_START(dialogWidth, pauseTextLength);
	wmove(dialog, 3, pauseTextX);
	wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(7, 0)));
	wattroff(dialog, A_BOLD);
	wprintw(dialog, pauseText.c_str());

	wgetch(dialog);

	Dialogs::destroyDialog(dialog);
}

void UiHandler::showMessage(std::string message) {
	int messageLength = message.length();
	int dialogWidth = messageLength + 4;

	WINDOW* dialog = Dialogs::createDialog(this->canvas, dialogWidth, 5);

	wmove(dialog, 1, 2);
	wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(15, 0)));
	wattron(dialog, A_BOLD);
	wprintw(dialog, message.c_str());

	std::string pauseText = PAUSE_TEXT;
	int pauseTextLength = pauseText.length();
	int pauseTextX = DIALOG_CENTER_START(dialogWidth, pauseTextLength);
	wmove(dialog, 3, pauseTextX);
	wattron(dialog, COLOR_PAIR(COLOR_PAIR_INDEX(7, 0)));
	wattroff(dialog, A_BOLD);
	wprintw(dialog, pauseText.c_str());

	wgetch(dialog);

	Dialogs::destroyDialog(dialog);
}

void UiHandler::showHelp() {
	std::vector<std::string> helpText = getHelpText();

	int minimumCursorY = this->canvas->getMinimumCursorY();
	int maximumCursorY = this->canvas->getMaximumCursorY();
	int minimumCursorX = this->canvas->getMinimumCursorX();
	int maximumCursorX = this->canvas->getMaximumCursorX();

	int padHeight = helpText.size();
	int dialogWidth = maximumCursorX - minimumCursorX + 1;
	int dialogHeight = maximumCursorY - minimumCursorY + 1;
	int dialogClientWidth = dialogWidth - 4;
	int dialogClientHeight = dialogHeight - 2;
	int dialogClientStartX = minimumCursorX + 2;
	int dialogClientStartY = minimumCursorY + 1;
	int dialogClientEndX = maximumCursorX - 2;
	int dialogClientEndY = maximumCursorY - 1;

	WINDOW* dialog = Dialogs::createDialog(this->canvas, dialogWidth, dialogHeight);
	wmove(dialog, dialogHeight - 1, 2);
	wprintw(
		dialog,
		" ESC = Exit Dialog %c %c = Scroll Up %c %c = Scroll Down ",
		0xB3,
		0x1E,
		0xB3,
		0x1F);
	wrefresh(dialog);

	WINDOW* dialogClient = newpad(padHeight, dialogClientWidth);
	scrollok(dialogClient, false);
	keypad(dialogClient, true);

	for (int i = 0; i < padHeight; i++) {
		std::string helpTextLine = helpText.at(i);
		wmove(dialogClient, i, 0);
		wprintw(dialogClient, helpTextLine.c_str());
	}

	int maxPadPositionY = padHeight - (dialogHeight - 2);
	int padPositionY = 0;

	bool exitDialog = false;
	while (!exitDialog) {
		prefresh(
			dialogClient,
			padPositionY,
			0,
			dialogClientStartY,
			dialogClientStartX,
			dialogClientEndY,
			dialogClientEndX);

		int inputKey = wgetch(dialogClient);
		if (KEY_UP == inputKey) {
			if (padPositionY > 0) {
				padPositionY--;
			}
		} else if (KEY_DOWN == inputKey) {
			if (padPositionY < maxPadPositionY) {
				padPositionY++;
			}
		} else if (COBALT_KEY_ESCAPE == inputKey) {
			exitDialog = true;
		}
	}

	Dialogs::destroyDialog(dialogClient);
	Dialogs::destroyDialog(dialog);
}
