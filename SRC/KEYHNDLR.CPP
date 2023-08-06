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
#include "KEYHNDLR.HPP"

void KeyHandler::handleKeyInput(int keyInput) {
	if (KEY_UP == keyInput) {
		this->canvas->moveUp();
	} else if (KEY_DOWN == keyInput) {
		this->canvas->moveDown();
	} else if (KEY_LEFT == keyInput) {
		this->canvas->moveLeft();
	} else if (KEY_RIGHT == keyInput) {
		this->canvas->moveRight();
	} else if (KEY_HOME == keyInput) {
		this->canvas->moveToHome();
	} else if (KEY_END == keyInput) {
		this->canvas->moveToEnd();
	} else if (ALT_UP == keyInput) {
		this->canvas->nextCharacterSet();
	} else if (ALT_DOWN == keyInput) {
		this->canvas->previousCharacterSet();
	} else if (CTL_UP == keyInput) {
		this->coloring->previousForegroundColor();
	} else if (CTL_DOWN == keyInput) {
		this->coloring->nextForegroundColor();
	} else if (CTL_LEFT == keyInput) {
		this->coloring->previousBackgroundColor();
	} else if (CTL_RIGHT == keyInput) {
		this->coloring->nextBackgroundColor();
	} else if (keyInput >= KEY_F(1) && keyInput <= KEY_F(10)) {
		this->canvas->drawCharacterFromSet(keyInput - KEY_F0 - 1);
	} else if (keyInput >= 32 && keyInput <= 126) {
		this->canvas->drawTypedCharacter(keyInput);
	} else if (COBALT_KEY_BACKSPACE == keyInput) {
		this->canvas->moveLeft();
		this->canvas->resetCharacter();
	} else if (KEY_DC == keyInput) {
		this->canvas->resetCharacter();
	} else if (COBALT_KEY_ENTER == keyInput) {
		this->canvas->moveToNextLine();
	} else if (COBALT_KEY_CTL_F == keyInput) {
		this->canvas->showFullDrawing();
	} else if (COBALT_KEY_CTL_N == keyInput) {
		this->fileHandler->newDrawing();
	} else if (COBALT_KEY_ESCAPE == keyInput) {
		if (this->uiHandler->getConfirmation("Exit Cobalt?")) {
			this->uiHandler->toggleExitApplication();
		}
	}
}
