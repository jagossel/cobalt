#include <curses.h>
#include "global.h"
#include "ui.h"

int main() {
	initscr();
	raw();
	keypad(stdscr, true);
	noecho();

	init();
	while (!exitApplication) {
		loadScreenSize();
		int keyInput = getch();
		handleKeyInput(keyInput);
		refresh();
	}

	endwin();
	
	return 0;
}

