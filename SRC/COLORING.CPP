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
#include "HEADERS/COLORING.HPP"

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

int Coloring::getCurrentForegroundColor() {
	return this->currentForegroundColor;
}

int Coloring::getCurrentBackgroundColor() {
	return this->currentBackgroundColor;
}
