#include <windows.h> 
#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <thread>
#include <cmath>
#include <chrono>
#include <cctype>
#include <cstdlib>
#include <algorithm>

using namespace std;

vector<vector<int>> out;
INPUT ip;

class minecraftEnums {
public:
	enum directions { BACKWARD = 0, FORWARD = 1, LEFT = 2, RIGHT = 3, DIAGONALBACKLEFT = 4, DIAGONALBACKRIGHT = 5, DIAGONALFORWARDLEFT = 6, DIAGONALFORWARDRIGHT = 7, COUNT };
	enum toJump { JUMP = 0, DONTJUMP = 1, JUMPCOUNT };
	enum toLook { LOOK = 0, DONTLOOK = 1, LOOKCOUNT };
};
class AUTvars {
private:
	bool enableSpecs = true;
	bool isForGrinding = true;
public:
	vector<char> possibleInputs = { 'Y', 'y', 'N', 'n' };
	bool hasViableInput(char &input) {
		return any_of(possibleInputs.begin(), possibleInputs.end(), [&input](char i) {return input == i;});
	}
	void setEnableSpecs(char input) {
		switch (input) {
		case 'Y':
			enableSpecs = true;
			break;
		case 'y':
			enableSpecs = true;
			break;
		case 'N':
			enableSpecs = false;
			break;
		case 'n':
			enableSpecs = false;
			break;
		}
	}
	void setForGrinding(char input) {
		switch (input) {
		case 'Y':
			isForGrinding = true;
			break;
		case 'y':
			isForGrinding = true;
			break;
		case 'N':
			isForGrinding = false;
			break;
		case 'n':
			isForGrinding = false;
			break;
		}
	}
	bool isEnabled() {
		return enableSpecs;
	}
	bool isGrinding() {
		return isForGrinding;
	}
	enum specialMoves {EMove = 0, RMove = 1, TMove = 3, YMove = 4, GMove=5, HMove=6, JMove=7, QMove=8, ZMove=9, VMove=10, SPECMOVESCOUNT};
	enum currentStand {
		StarPlatinum = 1, MagiciansRed = 2, SilverChariot = 3, Anubis = 4, ShadowDio = 5,
		KillerQueen = 6, CrazyDiamond = 7, TheHand = 8, GoldExperience = 9, KingCrimson = 10,
		StickyFingers = 11, WeatherReport = 12, SteelSpin = 13, WonderOfU = 14, D4C = 15,
		TheWorldHighVoltage = 16, TuskA1 = 17, TheWorld = 18, StarPlatinumTheWorld = 19, GoldExperienceRequiem = 20,
		CMoon = 21, MadeInHeaven = 22, D4CLoveTrain = 23, TuskA2 = 24, TuskA3 = 25,
		TuskA4 = 26, TheWorldOverHeaven = 27, SoftAndWet = 28, PlanetShaper = 29, Sol = 30,
		Nocturnus = 31, ShadowTheWorldRequiem = 32, StarPlatinumRequiem = 33, Xeno = 34, Locke = 35
	};
	// energy count coming later
	// !stand on!
	// finish later
	vector<vector<char>> standMovesets = {
		{'0'},// 0 is none
		{'E', 'R', 'T',  0 , 'G',  0 ,  0 ,  0 ,  0 ,  0 }, //SP (no cutscenes and timestop abilities)
		{'E', 'R', 'T', 'Y', 'G',  0 ,  0 ,  0 ,  0 ,  0 }, //MR (no long inputs)
		{'E', 'R', 'T', 'Y', 'G',  0 ,  0 ,  0 ,  0 ,  0 }, //SC
		{'E', 'R', 'T', 'Y',  0 ,  0 ,  0 ,  0 ,  0 ,  0 }, //Anubis
	};
};
void keyPress(char keyInput, int duration) {
	Sleep(50);
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = keyInput;
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));

	Sleep(duration);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

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

char setupSetupButton() {
	cout << "Print the position setup button (characters only), please." << endl;
	char ret;
	while (1) {
		cin >> ret;
		if ((ret >= 'A' && ret <= 'Z') || (ret >= 'a' && ret <= 'z')) {
			break;
		}
		else {
			cout << "Wrong input!" << endl;
		}
	}
	return ret;
}

void setupPos() {
	char setupChar = 0;
	POINT xypos;
	cout << "Enter amount of points, please : " << endl;
	int i = 3;
	cin >> i;
	setupChar = setupSetupButton();
	cout << "Setting up. Press '"<<setupChar<<"' to set the point for clicking, please" << endl;
	cout << "Select this window below and then press the setup button" << endl;
	cout << "Wait 3 sec please" << endl;
	Sleep(3000);
	//HideConsole();
	SetWindowPos(GetConsoleWindow(), NULL, 5000, 5000, 0, 0, 0);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	while (i > 0) {
		char g = _getch();
		if (g == setupChar || g==toupper(setupChar) || g==tolower(setupChar)) {
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

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(minecraftEnums::toLook::LOOKCOUNT) - 1);

	minecraftEnums::toLook randDir = static_cast<minecraftEnums::toLook>(distribution(generator));

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	int rand = randomMTInRange(0, 400);
	switch (randDir)
	{
	case minecraftEnums::LOOK:
		Sleep(100);
		cout << "	~LOOK" << endl;
		for (int i = 0; i <= rand / 24; i++) {
			Sleep(50);
			moveMouse(0 + i, 0);
		}

		//cout << "MX = " << cursorPos.x << " ; MY = " << cursorPos.y << endl;
		break;
	case minecraftEnums::DONTLOOK:
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

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(minecraftEnums::toJump::JUMPCOUNT) - 1);

	minecraftEnums::toJump randDir = static_cast<minecraftEnums::toJump>(distribution(generator));

	switch (randDir)
	{
	case minecraftEnums::JUMP:
		jump(walkDur);
		break;
	case minecraftEnums::DONTJUMP:
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

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(minecraftEnums::directions::DIAGONALBACKLEFT) - 1);

	minecraftEnums::directions randDir = static_cast<minecraftEnums::directions>(distribution(generator));

	switch (randDir)
	{
	case minecraftEnums::directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case minecraftEnums::directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case minecraftEnums::directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case minecraftEnums::directions::RIGHT:
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

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(minecraftEnums::directions::DIAGONALBACKLEFT) - 1);

	minecraftEnums::directions randDir = static_cast<minecraftEnums::directions>(distribution(generator));

	switch (randDir)
	{
	case minecraftEnums::directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case minecraftEnums::directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case minecraftEnums::directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case minecraftEnums::directions::RIGHT:
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

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(minecraftEnums::directions::DIAGONALBACKLEFT) - 1);

	minecraftEnums::directions randDir = static_cast<minecraftEnums::directions>(distribution(generator));

	switch (randDir)
	{
	case minecraftEnums::directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case minecraftEnums::directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case minecraftEnums::directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case minecraftEnums::directions::RIGHT:
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

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(minecraftEnums::directions::DIAGONALBACKLEFT) - 1);

	minecraftEnums::directions randDir = static_cast<minecraftEnums::directions>(distribution(generator));

	switch (randDir)
	{
	case minecraftEnums::directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case minecraftEnums::directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case minecraftEnums::directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "	Random2 ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "	Proceeding further2" << endl;
		break;
	case minecraftEnums::directions::RIGHT:
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

	std::uniform_int_distribution<int> distribution(0, static_cast<int>(minecraftEnums::directions::COUNT) - 1);

	minecraftEnums::directions randDir = static_cast<minecraftEnums::directions>(distribution(generator));
	cout << randDir << endl;
	switch (randDir)
	{
	case minecraftEnums::directions::BACKWARD:
		cout << "MOVING BACKWARD" << endl;
		randomLook();;
		walkBackward(walkDur);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case minecraftEnums::directions::FORWARD:
		cout << "MOVING FORWARD" << endl;
		randomLook();;
		walkForward(walkDur);
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case minecraftEnums::directions::LEFT:
		cout << "MOVING LEFT" << endl;
		randomLook();;
		walkLeft(walkDur);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case minecraftEnums::directions::RIGHT:
		cout << "MOVING RIGHT" << endl;
		randomLook();;
		walkRight(walkDur);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
		break;
	case minecraftEnums::directions::DIAGONALBACKLEFT:
		cout << "MOVING DIAGONALBACKLEFT" << endl;
		randomLook();;
		walkDiagonalBackLeft(walkDur, delayBetweenSteps);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
	case minecraftEnums::directions::DIAGONALBACKRIGHT:
		cout << "MOVING DIAGONALBACKRIGHT" << endl;
		randomLook();;
		walkDiagonalBackRight(walkDur, delayBetweenSteps);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
	case minecraftEnums::directions::DIAGONALFORWARDLEFT:
		cout << "MOVING DIAGONALFORWARDLEFT" << endl;
		randomLook();;
		walkDiagonalForwardLeft(walkDur, delayBetweenSteps);
		cout << "Random ended" << endl;
		Sleep(delayBetweenSteps);
		cout << "Proceeding further" << endl;
	case minecraftEnums::directions::DIAGONALFORWARDRIGHT:
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

// 910 repeats for 8 or 9 or 10 hours
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

void specialMovesLoop(char choice) {
	AUTvars aut;
	if (aut.isEnabled()) {
		cout << "Random started" << endl;

		std::mt19937 generator(std::time(nullptr));

		std::uniform_int_distribution<int> distribution(0, static_cast<int>(AUTvars::specialMoves::SPECMOVESCOUNT) - 1);

		AUTvars::specialMoves randMove = static_cast<AUTvars::specialMoves>(distribution(generator));
		cout << randMove << endl;
		switch (randMove)
		{
		case AUTvars::EMove:
			keyPress('E', 2000);
			break;
		case AUTvars::RMove:
			keyPress('R', 1000);
			break;
		case AUTvars::TMove:
			keyPress('T', 1000);
			break;
		case AUTvars::YMove:
			keyPress('Y', 1000);
			break;
		default:
			break;
		}
	}
}
void AUTTreePunchBot() {
	system("cls");
	AUTvars aut;
	int amount;
	char choice;

	cout << "Is grinding Locaca Tree ? " << endl;
	cout << "Enter 'Y' for yes or 'N' for no" << endl;
	while (1) {
		cin >> choice;
		if (aut.hasViableInput(choice)) {
			aut.setForGrinding(choice);
			break;
		}
		else {
			cout << endl << "Wrong input!" << endl;
		}
	}

	if (aut.isGrinding()) {
		cout << "Include special moves? (Set 'Yes' as default)" << endl;
		cout << "Enter 'Y' for yes or 'N' for no" << endl;
		while (1) {
			cin >> choice;
			if (aut.hasViableInput(choice)) {
				aut.setEnableSpecs(choice);
				break;
			}
			else {
				cout << endl << "Wrong input!" << endl;
			}
		}
		cout << "Enter amount of repeats : ";
		cin >> amount;
		cout << "Wait 5 seconds..." << endl;
		Sleep(5000);
		for (int i = 0; i < amount; i++) {
			cout << "Current cycle = " << i << endl;
			click();
			Sleep(100);
			specialMovesLoop(choice);
			Sleep(50);
		}
	}
	if (!aut.isGrinding()) {
		cout << "Enter amount of repeats : ";
		cin >> amount;
		cout << "Wait 5 seconds..." << endl;
		Sleep(5000);
		for (int i = 0; i < amount; i++) {
			cout << "Current cycle = " << i << endl;
			RandomWalk(1000, 500, 0);
			Sleep(50);
		}
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