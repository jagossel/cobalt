void redrawRegion(int startX, int startY, int endX, int endY);

void redrawRegion(int startX, int startY, int endX, int endY) {
	curs_set(false);
	for (int redrawY = startY; redrawY <= endY; redrawY++) {
		for (int redrawX = startX; redrawX <= endX; redrawX++) {
			move(redrawY, redrawX);
			int yIndex = redrawY - minimumCursorY;
			int xIndex = redrawX - minimumCursorX;
			printw("%c", canvas[yIndex][xIndex].character);
		}
	}

	move(currentCursorY, currentCursorX);
	curs_set(true);
}
