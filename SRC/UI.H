void init();
void handleKeyInput(int keyInput);
void loadScreenSize();
void updateStatusBar();

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

void updateStatusBar() {
	int toggleStatusBarMoveY = statusBarAtTop ?
		minimumCursorY + statusBarYTolerance :
		maximumCursorY - statusBarYTolerance;

	bool moveStatusBar = false;
	if (statusBarAtTop) {
		if (currentCursorY <= toggleStatusBarMoveY) {
			statusBarAtTop = false;
			moveStatusBar = true;
		}
	} else {
		if (currentCursorY >= toggleStatusBarMoveY) {
			statusBarAtTop = true;
			moveStatusBar = true;
		}
	}

	int statusBarY = statusBarAtTop ? minimumCursorY : maximumCursorY;
	if (moveStatusBar) {
		if (statusBarAtTop) {
			redrawRegion(minimumCursorX, maximumCursorY,
				maximumCursorX, maximumCursorY);
		} else {
			redrawRegion(minimumCursorX, minimumCursorY,
				maximumCursorX, minimumCursorY);
		}
	}

	curs_set(false);
	move(statusBarY, minimumCursorX);

	int printCurrentCursorX = currentCursorX + 1;
	int printCurrentCursorY = currentCursorY + 1;
	printw("%02d,%02d",
		printCurrentCursorX,
		printCurrentCursorY);
	curs_set(true);
	move(currentCursorY, currentCursorX);
}

