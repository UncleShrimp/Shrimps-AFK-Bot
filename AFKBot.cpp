#include <windows.h> 
#include <iostream>
#include <conio.h>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <random>
#include <thread>
#include <cmath>
#include <chrono>

using namespace std;

vector<vector<int>> out;
INPUT ip;

enum directions { BACKWARD = 0, FORWARD = 1, LEFT = 2, RIGHT = 3, DIAGONALBACKLEFT = 4, DIAGONALBACKRIGHT = 5, DIAGONALFORWARDLEFT = 6, DIAGONALFORWARDRIGHT = 7, COUNT };
enum toJump { JUMP = 0, DONTJUMP = 1, JUMPCOUNT };
enum toLook { LOOK = 0, DONTLOOK = 1, LOOKCOUNT };

int randomMTInRange(int start, int end) {
	std::random_device rd; // Obtain a random seed from the OS
	std::mt19937 gen(rd()); // Initialize the Mersenne Twister engine with the seed

	// Define the range
	int min_val = start;
	int max_val = end;

	// Create a uniform integer distribution within the specified range
	std::uniform_int_distribution<> distrib(min_val, max_val);

	// Generate and print a random number
	int random_number = distrib(gen);
	std::cout << "		Random LOOK number between " << min_val << " and " << max_val << ": " << random_number << std::endl;

	return random_number;
}

void setupPos() {
	POINT xypos;
	cout << "Enter amount of points, please : " << endl;
	int i = 3;
	cin >> i;
	cout << "Setting up. Press 'l' to set the point for clicking, please" << endl;
	cout << "Wait 3 sec please" << endl;
	Sleep(3000);
	//HideConsole();
	SetWindowPos(GetConsoleWindow(), NULL, 5000, 5000, 0, 0, 0);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	while (i > 0) {
		char g = _getch();
		if (g == 'L' || g == 'l') {
			GetCursorPos(&xypos);
			cout << "Setup Num " << i << "; Set X = " << xypos.x << "; Set Y = " << xypos.y << endl;
			vector<int> t = { xypos.x , xypos.y };
			out.push_back(t);
			i--;
		}
	}
}

void moveMouse(int x, int y) {
	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
	double fx = x * (65535.0f / fScreenWidth);
	double fy = y * (65535.0f / fScreenHeight);
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE;
	Input.mi.dx = fx;
	Input.mi.dy = fy;
	::SendInput(1, &Input, sizeof(INPUT));
}

bool finishedTurning = true;

void smoothMoveMouse(int x, int y, int delay, int repeats) {
	int steps = 0;
	switch (finishedTurning) {
	case true:
		finishedTurning = false;
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);
		for (int i = 0; i < repeats; i++) {
			Sleep(delay);
			if (x > currentMousePos.x) {
				currentMousePos.x++;
			}
			if (x < currentMousePos.x) {
				currentMousePos.x--;
			}
			moveMouse(currentMousePos.x, 500);
		}
		finishedTurning = true;
		break;
	case false:
		break;
	}

}

void randomLook() {
	cout << "			~Random4 started" << endl;

	std::mt19937 generator(std::time(nullptr));

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(toLook::LOOKCOUNT) - 1);

	toLook randDir = static_cast<toLook>(distribution(generator));

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	int rand = randomMTInRange(0, 400);
	switch (randDir)
	{
	case LOOK:
		Sleep(100);
		cout << "	~LOOK" << endl;
		for (int i = 0; i <= rand / 24; i++) {
			Sleep(50);
			moveMouse(0 + i, 0);
		}

		//cout << "MX = " << cursorPos.x << " ; MY = " << cursorPos.y << endl;
		break;
	case DONTLOOK:
		break;
	default:
		break;
	}
}

void click() {
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	cout << "CLICK \n";
}

void antiLeave() {
	Sleep(500);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(500);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(500);
}

void longwait(int lenght) {
	for (int time = 0; time < lenght; time++) {
		antiLeave();
		Sleep(1);
	}
}

string line;
string searchMatchStart = "Dialog sound playing complete:";
string testFind = "a";// test variable to search in file

void SendEscapeToWindow(HWND hwnd) {
	PostMessage(hwnd, WM_KEYDOWN, VK_ESCAPE, 0);
	Sleep(50);
	PostMessage(hwnd, WM_KEYUP, VK_ESCAPE, 0xC0000001);
}

void escapeWindow() {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = VK_ESCAPE;
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	Sleep(10);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

void jump(int walkDur) {
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = VK_SPACE;
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	cout << "			Jump" << endl;
	Sleep(walkDur);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

void randomJump(int walkDur) {
	cout << "			Random3 started" << endl;

	std::mt19937 generator(std::time(nullptr));

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(toJump::JUMPCOUNT) - 1);

	toJump randDir = static_cast<toJump>(distribution(generator));

	switch (randDir)
	{
	case JUMP:
		jump(walkDur);
		break;
	case DONTJUMP:
		break;
	default:
		break;
	}
}

void walkForward(int walkDur) {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = 'W';
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	Sleep(walkDur);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

void walkBackward(int walkDur) {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = 'S';
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));

	randomJump(walkDur);

	Sleep(walkDur);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

void walkRight(int walkDur) {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = 'D';
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));


	randomJump(walkDur);

	Sleep(walkDur);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

void walkLeft(int walkDur) {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = 'A';
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));


	randomJump(walkDur);

	Sleep(walkDur);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

void walkDiagonalBackLeft(int walkDur, int delayBetweenSteps) {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = 'S';
	SHORT key2;
	UINT mappedkey2;
	INPUT input2 = { 0 };
	key2 = 'A';
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	mappedkey2 = MapVirtualKey(LOBYTE(key2), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	input2.type = INPUT_KEYBOARD;
	input2.ki.dwFlags = KEYEVENTF_SCANCODE;
	input2.ki.wScan = mappedkey2;

	SendInput(1, &input, sizeof(input));
	SendInput(1, &input, sizeof(input2));


	randomJump(walkDur);

	Sleep(walkDur);

	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
	input2.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input2, sizeof(input2));

	cout << "	Random2 started" << endl;

	std::mt19937 generator(std::time(nullptr));

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(directions::DIAGONALBACKLEFT) - 1);

	directions randDir = static_cast<directions>(distribution(generator));

	switch (randDir)
	{
	case directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case directions::RIGHT:
		cout << "MOVING RIGHT" << endl;
		randomLook();;
		walkRight(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	}
}
void walkDiagonalBackRight(int walkDur, int delayBetweenSteps) {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = 'S';
	SHORT key2;
	UINT mappedkey2;
	INPUT input2 = { 0 };
	key2 = 'D';
	mappedkey = MapVirtualKey(LOBYTE(key), 0);mappedkey2 = MapVirtualKey(LOBYTE(key2), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;

	input2.type = INPUT_KEYBOARD;
	input2.ki.dwFlags = KEYEVENTF_SCANCODE;
	input2.ki.wScan = mappedkey2;

	SendInput(1, &input, sizeof(input));
	SendInput(1, &input, sizeof(input2));


	randomJump(walkDur);

	Sleep(walkDur);

	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
	input2.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input2, sizeof(input2));

	cout << "	Random2 started" << endl;

	std::mt19937 generator(std::time(nullptr));

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(directions::DIAGONALBACKLEFT) - 1);

	directions randDir = static_cast<directions>(distribution(generator));

	switch (randDir)
	{
	case directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case directions::RIGHT:
		cout << "MOVING RIGHT" << endl;
		randomLook();;
		walkRight(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	}
}
void walkDiagonalForwardLeft(int walkDur, int delayBetweenSteps) {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = 'W';
	SHORT key2;
	UINT mappedkey2;
	INPUT input2 = { 0 };
	key2 = 'A';
	mappedkey = MapVirtualKey(LOBYTE(key), 0);mappedkey2 = MapVirtualKey(LOBYTE(key2), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;

	input2.type = INPUT_KEYBOARD;
	input2.ki.dwFlags = KEYEVENTF_SCANCODE;
	input2.ki.wScan = mappedkey2;

	SendInput(1, &input, sizeof(input));
	SendInput(1, &input, sizeof(input2));


	randomJump(walkDur);

	Sleep(walkDur);

	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
	input2.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input2, sizeof(input2));

	cout << "	Random2 started" << endl;

	std::mt19937 generator(std::time(nullptr));

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(directions::DIAGONALBACKLEFT) - 1);

	directions randDir = static_cast<directions>(distribution(generator));

	switch (randDir)
	{
	case directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case directions::RIGHT:
		cout << "MOVING RIGHT" << endl;
		randomLook();;
		walkRight(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	}
}
void walkDiagonalForwardRight(int walkDur, int delayBetweenSteps) {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = 'W';
	SHORT key2;
	UINT mappedkey2;
	INPUT input2 = { 0 };
	key2 = 'D';
	mappedkey = MapVirtualKey(LOBYTE(key), 0);mappedkey2 = MapVirtualKey(LOBYTE(key2), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;

	input2.type = INPUT_KEYBOARD;
	input2.ki.dwFlags = KEYEVENTF_SCANCODE;
	input2.ki.wScan = mappedkey2;

	SendInput(1, &input, sizeof(input));
	SendInput(1, &input, sizeof(input2));


	randomJump(walkDur);

	Sleep(walkDur);

	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
	input2.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input2, sizeof(input2));

	cout << "	Random2 started" << endl;

	std::mt19937 generator(std::time(nullptr));

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(directions::DIAGONALBACKLEFT) - 1);

	directions randDir = static_cast<directions>(distribution(generator));

	switch (randDir)
	{
	case directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case directions::RIGHT:
		cout << "MOVING RIGHT" << endl;
		randomLook();;
		walkRight(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	}
}

void RandomWalk(int duration, int walkDur, int delayBetweenSteps) {
	Sleep(duration);
	cout << "Random started" << endl;

	std::mt19937 generator(std::time(nullptr));

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(directions::COUNT) - 1);

	directions randDir = static_cast<directions>(distribution(generator));
	cout << randDir << endl;
	switch (randDir)
	{
	case directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case directions::RIGHT:
		cout << "MOVING RIGHT" << endl;
		randomLook();;
		walkRight(walkDur);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case directions::DIAGONALBACKLEFT:
		cout << "MOVING DIAGONALBACKLEFT" << endl;
		randomLook();;
		walkDiagonalBackLeft(walkDur, delayBetweenSteps);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
	case directions::DIAGONALBACKRIGHT:
		cout << "MOVING DIAGONALBACKRIGHT" << endl;
		randomLook();;
		walkDiagonalBackRight(walkDur, delayBetweenSteps);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
	case directions::DIAGONALFORWARDLEFT:
		cout << "MOVING DIAGONALFORWARDLEFT" << endl;
		randomLook();;
		walkDiagonalForwardLeft(walkDur, delayBetweenSteps);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
	case directions::DIAGONALFORWARDRIGHT:
		cout << "MOVING DIAGONALFORWARDRIGHT" << endl;
		randomLook();;
		walkDiagonalForwardRight(walkDur, delayBetweenSteps);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
	default:
		break;
	}
}

void minecraft() {
	system("cls");
	moveMouse(1000, 1000);
	SetWindowPos(GetConsoleWindow(), NULL, 100, 100, 1000, 300, 0);
	ShowWindow(GetConsoleWindow(), SW_SHOW);

	POINT xypos;
	int amount;
	int delay;
	cout << "Enter amount of repeats, please : \n";
	cin >> amount;
	cout << "Enter delay between clicks, please : \n";
	cin >> delay;
	setupPos();
	cout << "Amount of repeats : ";
	cout << out.size() << endl;
	cout << "Delay between clicks : " << endl;
	cout << delay;
	cout << endl;

	SetWindowPos(GetConsoleWindow(), NULL, 100, 100, 1000, 300, 0);
	ShowWindow(GetConsoleWindow(), SW_SHOW);

	system("pause");
	SetWindowPos(GetConsoleWindow(), NULL, 1200, 0, 300, 300, 0);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	SetCursorPos(out[0][0], out[0][1]);
	click();
	Sleep(200);
	for (int i = 0; i < amount; i++) {
		cout << endl << endl << "Current cycle = " << i << endl;
		for (int setP = 0; setP < out.size(); setP++) {
			SetCursorPos(out[setP][0], out[setP][1]);
			cout << "X = " << out[setP][0] << "; Y = " << out[setP][1] << endl;
			click();
			Sleep(delay);
			escapeWindow();
			Sleep(delay);
			click();
			Sleep(delay);
			for (int r = 0; r < 20; r++) {
				RandomWalk(1000, delay, 0);
			}
			escapeWindow();
			Sleep(delay);
		}
		cout << endl;
		if (amount > 1) {
			Sleep(300);
		}
	}
	Sleep(10);
	SetWindowPos(GetConsoleWindow(), NULL, 100, 100, 1000, 300, 0);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	cout << "\n Finished executing!" << endl;
	Sleep(2000);
	exit(0);

	Sleep(5000);
	// Simulate a key press
	HWND gameWindow = FindWindowA(NULL, "Warface");
	if (gameWindow) {
		SendEscapeToWindow(gameWindow);
	}
	cout << "Pressed";

}

// 910 repeats for 8 or 9 or 10 hours

void AUTTreePunchBot() {
	system("cls");
	int amount;
	cout << "Enter amount of repeats : ";
	cin >> amount;
	cout << "Wait 5 seconds..." << endl;
	Sleep(5000);
	for (int i = 0; i < amount; i++) {
		cout << "Current cycle = " << i << endl;
		click();
		Sleep(50);
	}
}
void selectBot() {
	system("cls");
	cout << "   _______  _______  __  __      ______  _______  _______  \n";
	cout << "  |   _   ||    ___||  |/  |    |   __  |       ||_     _| \n";
	cout << "  |       ||    ___||     <     |   __ <|   -   |  |   |   \n";
	cout << "  |___|___||___|    |__| __|    |______/|_______|  |___|   \n";

	cout << " by Uncle._.Shrimp\n\n";
	cout << "Pick bot to use : \n1 - Minecraft Prisoner\n2 - AUT Tree punch" << endl;
	int botPick;
	cin >> botPick;
	switch (botPick) {
	case 1:
		cout << endl << "Picked Minecraft Prisoner bot" << endl << endl;
		Sleep(1000);
		minecraft();
		break;
	case 2:
		cout << endl << "Picked AUT Tree punch bot" << endl << endl;
		Sleep(1000);
		AUTTreePunchBot();
		break;
	default:
		cout << endl << "Unknown selection" << endl << endl;
		Sleep(1000);
		selectBot();
		break;
	}
}
int main() {
	selectBot();
}