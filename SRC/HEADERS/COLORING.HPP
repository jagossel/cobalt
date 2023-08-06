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
#ifndef COLORING_H
#define COLORING_H

class Coloring {
	const int minimumForegroundColor = 0;
	const int maximumForegroundColor = 15;

	const int minimumBackgroundColor = 0;
	const int maximumBackgroundColor = 15;

	int currentForegroundColor;
	int currentBackgroundColor;

	public:
		Coloring():
			currentForegroundColor(7),
			currentBackgroundColor(0) { }
		void previousForegroundColor();
		void nextForegroundColor();
		void previousBackgroundColor();
		void nextBackgroundColor();
		void reset();
		int getCurrentForegroundColor();
		int getCurrentBackgroundColor();
};

#endif
