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
#include "HEADERS/CANVAS.HPP"

void Canvas::resizeCanvas() {
	int minX, minY, maxX, maxY;
	getbegyx(stdscr, minY, minX);
	getmaxyx(stdscr, maxY, maxX);

	// Not sure why this needs to be done, but the maximum range has to be decremented in order to keep the cursor
	// from sticking to the bottom of the screen.
	maxX--;
	maxY--;

	this->minimumCursorX = minX;
	if (this->currentCursorX <= this->minimumCursorX) {
		this->currentCursorX = this->minimumCursorX;
	}

	this->maximumCursorX = maxX;
	if (this->currentCursorX >= this->maximumCursorX) {
		this->currentCursorX = this->maximumCursorX;
	}

	this->minimumCursorY = minY;
	if (this->currentCursorY <= this->minimumCursorY) {
		this->currentCursorY = this->minimumCursorY;
	}

	this->maximumCursorY = maxY;
	if (this->currentCursorY >= this->maximumCursorY) {
		this->currentCursorY = this->maximumCursorY;
	}
}

void Canvas::moveUp() {
	if (this->currentCursorY <= this->minimumCursorY) return;
	this->currentCursorY--;
	move(this->currentCursorY, this->currentCursorX);
}

void Canvas::moveDown() {
	if (this->currentCursorY >= this->maximumCursorY) return;
	this->currentCursorY++;
	move(this->currentCursorY, this->currentCursorX);
}

void Canvas::moveLeft() {
	if (this->currentCursorX <= this->minimumCursorX) return;
	this->currentCursorX--;
	move(this->currentCursorY, this->currentCursorX);
}

void Canvas::moveRight() {
	if (this->currentCursorX >= this->maximumCursorX) return;
	this->currentCursorX++;
	move(this->currentCursorY, this->currentCursorX);
}

void Canvas::moveToHome() {
	this->currentCursorX = this->minimumCursorX;
	move(this->currentCursorY, this->currentCursorX);
}

void Canvas::moveToEnd() {
	this->currentCursorX = this->maximumCursorX;
	move(this->currentCursorY, this->currentCursorX);
}

void Canvas::moveToNextLine() {
	this->moveDown();
	this->moveToHome();
}

void Canvas::previousCharacterSet() {
	if (this->currentCharacterSetIndex <= this->minimumCharacterSetIndex) {
		this->currentCharacterSetIndex = this->maximumCharacterSetIndex;
	} else {
		this->currentCharacterSetIndex--;
	}
}

void Canvas::nextCharacterSet() {
	if (this->currentCharacterSetIndex >= this->maximumCharacterSetIndex) {
		this->currentCharacterSetIndex = this->minimumCharacterSetIndex;
	} else {
		this->currentCharacterSetIndex++;
	}
}

void Canvas::reset() {
	this->currentCursorX = minimumCursorX;
	this->currentCursorY = minimumCursorY;
	this->currentCharacterSetIndex = minimumCharacterSetIndex;
}

int Canvas::getMinimumCursorX() {
	return this->minimumCursorX;
}

int Canvas::getCurrentCursorX() {
	return this->currentCursorX;
}

int Canvas::getMaximumCursorX() {
	return this->maximumCursorX;
}

int Canvas::getMinimumCursorY() {
	return this->minimumCursorY;
}

int Canvas::getCurrentCursorY() {
	return this->currentCursorY;
}

int Canvas::getMaximumCursorY() {
	return this->maximumCursorY;
}

int Canvas::getCurrentCharacterSetIndex() {
	return this->currentCharacterSetIndex;
}

char Canvas::getCharacterFromSet(int characterIndex) {
	return this->characterSets[this->currentCharacterSetIndex][characterIndex];
}
