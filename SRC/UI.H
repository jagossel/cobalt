void init();
void loadScreenSize();
void handleKeyInput(int keyInput);
void updateStatusBar();

void init() {
	loadScreenSize();
	currentCursorY = minimumCursorY;
	currentCursorX = minimumCursorX;
}

void loadScreenSize() {
	getbegyx(stdscr, minimumCursorY, minimumCursorX);
	getmaxyx(stdscr, maximumCursorY, maximumCursorX);

	// Not sure why this needs to be done, but the maximum range has to be decremented in order to keep the cursor
	// from sticking to the bottom of the screen.
	maximumCursorY--;
	maximumCursorX--;
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
	} else if (ALT_UP == keyInput) {
		if (currentCharacterSetIndex >= maximumCharacterSetIndex) {
			currentCharacterSetIndex = minimumCharacterSetIndex;
		} else {
			currentCharacterSetIndex++;
		}
	} else if (ALT_DOWN == keyInput) {
		if (currentCharacterSetIndex <= minimumCharacterSetIndex) {
			currentCharacterSetIndex = maximumCharacterSetIndex;
		} else {
			currentCharacterSetIndex--;
		}
	} else if (27 == keyInput) {
		exitApplication = true;
	}

	if (moveCursor) {
		move(currentCursorY, currentCursorX);
	}
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
	printw(
		"%02d,%02d \xB3 F1=%c F2=%c F3=%c F4=%c F5=%c F6=%c F7=%c F8=%c F9=%c F10=%c",
		printCurrentCursorX,
		printCurrentCursorY,
		characterSets[currentCharacterSetIndex][0],
		characterSets[currentCharacterSetIndex][1],
		characterSets[currentCharacterSetIndex][2],
		characterSets[currentCharacterSetIndex][3],
		characterSets[currentCharacterSetIndex][4],
		characterSets[currentCharacterSetIndex][5],
		characterSets[currentCharacterSetIndex][6],
		characterSets[currentCharacterSetIndex][7],
		characterSets[currentCharacterSetIndex][8],
		characterSets[currentCharacterSetIndex][9]);

	curs_set(true);
	move(currentCursorY, currentCursorX);
}
