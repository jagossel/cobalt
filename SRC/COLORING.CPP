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
#include "COLORING.HPP"
#include "MACROS.HPP"

void Coloring::initColors() {
	for (int bg = this->minimumBackgroundColor; bg <= this->maximumBackgroundColor; bg++) {
		int backgroundColorIndex = bg % 8;
		short backgroundColor = colorList[backgroundColorIndex];
		for (int fg = this->minimumForegroundColor; fg <= this->maximumForegroundColor; fg++) {
			int foregroundColorIndex = fg % 8;
			int colorPairIndex = COLOR_PAIR_INDEX(fg, bg);
			short foregroundColor = colorList[foregroundColorIndex];

			init_pair(colorPairIndex, foregroundColor, backgroundColor);
		}
	}
}

void Coloring::previousForegroundColor() {
	if (this->currentForegroundColor <= this->minimumForegroundColor) {
		this->currentForegroundColor = this->maximumForegroundColor;
	} else {
		this->currentForegroundColor--;
	}
}

void Coloring::nextForegroundColor() {
	if (this->currentForegroundColor >= this->maximumForegroundColor) {
		this->currentForegroundColor = this->minimumForegroundColor;
	} else {
		this->currentForegroundColor++;
	}
}

void Coloring::previousBackgroundColor() {
	if (this->currentBackgroundColor <= this->minimumBackgroundColor) {
		this->currentBackgroundColor = this->maximumBackgroundColor;
	} else {
		this->currentBackgroundColor--;
	}
}

void Coloring::nextBackgroundColor() {
	if (this->currentBackgroundColor >= this->maximumBackgroundColor) {
		this->currentBackgroundColor = this->minimumBackgroundColor;
	} else {
		this->currentBackgroundColor++;
	}
}

void Coloring::reset() {
	this->currentForegroundColor = 7;
	this->currentBackgroundColor = 0;
}

void Coloring::applyColoring() {
	this->setColors(this->currentForegroundColor, this->currentBackgroundColor);
}

void Coloring::setColors(int foregroundColor, int backgroundColor) {
	int colorPairIndex = COLOR_PAIR_INDEX(foregroundColor, backgroundColor);
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

void Coloring::resetColors() {
	int colorPairIndex = COLOR_PAIR_INDEX(7, 0);
	attroff(COLOR_PAIR(colorPairIndex));
	attroff(A_BOLD);
	attroff(A_BLINK);
}

int Coloring::getMinimumForegroundColor() {
	return this->minimumForegroundColor;
}

int Coloring::getCurrentForegroundColor() {
	return this->currentForegroundColor;
}

int Coloring::getMaximumForegroundColor() {
	return this->maximumForegroundColor;
}

int Coloring::getMinimumBackgroundColor() {
	return this->minimumBackgroundColor;
}

int Coloring::getCurrentBackgroundColor() {
	return this->currentBackgroundColor;
}

int Coloring:: getMaximumBackgroundColor() {
	return this->maximumBackgroundColor;
}

void Coloring::setCurrentForegroundColor(int value) {
	this->currentForegroundColor = value;
}

void Coloring::setCurrentBackgroundColor(int value) {
	this->currentBackgroundColor = value;
}
