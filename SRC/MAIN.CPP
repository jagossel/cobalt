#include <curses.h>
#include "global.h"
#include "drawing.h"
#include "ui.h"
#include "fileio.h"

int main() {
	initscr();
	raw();
	keypad(stdscr, true);
	noecho();

	init();
	initDrawing();
	redrawRegion(minimumCursorX, minimumCursorY,
		maximumCursorX, maximumCursorY);

	while (!exitApplication) {
		loadScreenSize();
		int keyInput = getch();
		handleKeyInput(keyInput);
		refresh();
	}

	endwin();
	
	return 0;
}

