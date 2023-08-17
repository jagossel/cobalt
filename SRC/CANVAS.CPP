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
#include <vector>
#include <CURSES.H>
#include "DRAWCHAR.HPP"
#include "CANVAS.HPP"

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

int Canvas::getMinimumCharacterSetIndex() {
	return this->minimumCharacterSetIndex;
}

int Canvas::getCurrentCharacterSetIndex() {
	return this->currentCharacterSetIndex;
}

void Canvas::setCurrentCharacterSetIndex(int value) {
	this->currentCharacterSetIndex = value;
}

int Canvas::getMaximumCharacterSetIndex() {
	return this->maximumCharacterSetIndex;
}

char Canvas::getCharacterFromSet(int characterIndex) {
	return this->characterSets[this->currentCharacterSetIndex][characterIndex];
}

char Canvas::getCharacterFromSet(int characterSetIndex, int characterIndex) {
	return this->characterSets[characterSetIndex][characterIndex];
}

void Canvas::drawCharacterFromSet(int characterIndex) {
	this->drawTypedCharacter(this->characterSets[this->currentCharacterSetIndex][characterIndex]);
}

void Canvas::drawTypedCharacter(char typedCharacter) {
	int currentForegroundColor = this->coloring->getCurrentForegroundColor();
	int currentBackgroundColor = this->coloring->getCurrentBackgroundColor();

	int canvasIndex = CANVAS_INDEX(this->currentCursorY, this->currentCursorX);
	this->drawingCanvas[canvasIndex].character = typedCharacter;
	this->drawingCanvas[canvasIndex].foregroundColor = currentForegroundColor;
	this->drawingCanvas[canvasIndex].backgroundColor = currentBackgroundColor;

	this->coloring->applyColoring();
	printw("%c", typedCharacter);
	this->coloring->resetColors();
	this->moveRight();
}

void Canvas::resetCharacter() {
	int canvasIndex = CANVAS_INDEX(this->currentCursorY, this->currentCursorX);
	this->drawingCanvas[canvasIndex].character = ' ';
	this->drawingCanvas[canvasIndex].foregroundColor = 7;
	this->drawingCanvas[canvasIndex].backgroundColor = 0;

	this->coloring->setColors(7, 0);
	printw("%c", ' ');
}

void Canvas::showFullDrawing() {
	this->redrawAll();
	curs_set(0);
	getch();
	curs_set(1);
}

void Canvas::redrawAll() {
	this->redrawRegion(this->minimumCursorX, this->minimumCursorY, this->maximumCursorX, this->maximumCursorY);
}

void Canvas::redrawRegion(int startX, int startY, int endX, int endY) {
	curs_set(0);
	for (int redrawY = startY; redrawY <= endY; redrawY++) {
		for (int redrawX = startX; redrawX <= endX; redrawX++) {
			int yIndex = redrawY - this->minimumCursorY;
			int xIndex = redrawX - this->minimumCursorX;

			int canvasIndex = CANVAS_INDEX(yIndex, xIndex);
			int foregroundColor = this->drawingCanvas[canvasIndex].foregroundColor;
			int backgroundColor = this->drawingCanvas[canvasIndex].backgroundColor;

			move(redrawY, redrawX);
			this->coloring->setColors(foregroundColor, backgroundColor);
			printw("%c", drawingCanvas[canvasIndex].character);
		}
	}

	this->coloring->resetColors();
	move(this->currentCursorY, this->currentCursorX);
	curs_set(1);
}

void Canvas::initializeDrawing() {
	this->drawingCanvas.clear();
	for (int initY = this->minimumCursorY; initY <= this->maximumCursorY; initY++) {
		for (int initX = this->minimumCursorX; initX <= this->maximumCursorX; initX++) {
			DrawingCharacter characterData;
			characterData.character = ' ';
			characterData.foregroundColor = 7;
			characterData.backgroundColor = 0;

			this->drawingCanvas.push_back(characterData);
		}
	}

	this->redrawAll();
}

std::vector<DrawingCharacter>* Canvas::getDrawing() {
	return &this->drawingCanvas;
}
