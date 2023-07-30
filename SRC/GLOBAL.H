using namespace std;

class DrawingCharacter {
	public:
		char character;
		int foregroundColor;
		int backgroundColor;
};

const int statusBarYTolerance = 2;
const int minimumCharacterSetIndex = 0;
const int maximumCharacterSetIndex = 10;

bool exitApplication = false;
bool statusBarAtTop = false;

int minimumCursorX = 0;
int minimumCursorY = 0;

int maximumCursorX = 0;
int maximumCursorY = 0;

int currentCursorX = 0;
int currentCursorY = 0;

int currentCharacterSetIndex = 0;

DrawingCharacter canvas[25][80];
