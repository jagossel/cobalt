void init();
void handleKeyInput(int keyInput);
void loadScreenSize();

void init() {
	loadScreenSize();
	currentCursorY = minimumCursorY;
	currentCursorX = minimumCursorX;
}

void handleKeyInput(int keyInput) {
	bool moveCursor = false;
	if (KEY_UP == keyInput) {
		if (currentCursorY <= minimumCursorY) return;
		currentCursorY--;
		moveCursor = true;
	} else if (KEY_DOWN == keyInput) {
		if (currentCursorY >= maximumCursorY) return;
		currentCursorY++;
		moveCursor = true;
	} else if (KEY_LEFT == keyInput) {
		if (currentCursorX <= minimumCursorX) return;
		currentCursorX--;
		moveCursor = true;
	} else if (KEY_RIGHT == keyInput) {
		if (currentCursorX >= maximumCursorX) return;
		currentCursorX++;
		moveCursor = true;
	} else if (27 == keyInput) {
		exitApplication = true;
	}

	if (moveCursor) {
		move(currentCursorY, currentCursorX);
	}
}

void loadScreenSize() {
	getbegyx(stdscr, minimumCursorY, minimumCursorX);
	getmaxyx(stdscr, maximumCursorY, maximumCursorX);

	// Not sure why this needs to be done, but the maximum range has
	// to be decremented in order to keep the cursor from sticking
	// to the bottom of the screen.
	maximumCursorY--;
	maximumCursorX--;
}

