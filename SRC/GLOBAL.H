using namespace std;

class DrawingCharacter {
	public:
		char character;
		int foregroundColor;
		int backgroundColor;
};

bool exitApplication = false;
int minimumCursorX = 0;
int minimumCursorY = 0;

int maximumCursorX = 0;
int maximumCursorY = 0;

int currentCursorX = 0;
int currentCursorY = 0;

DrawingCharacter canvas[25][80];

