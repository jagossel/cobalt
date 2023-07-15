void initDrawing() {
	for (int initY = 0; initY < 25; initY++) {
		for (int initX = 0; initX < 80; initX++) {
			canvas[initY][initX].character = ' ';
			canvas[initY][initX].foregroundColor = 7;
			canvas[initY][initX].backgroundColor = 0;
		}
	}
}

