#pragma once
#include "Menu.h"

class MainMenu : public Menu
{
public:
	static const int IND_OnePlayer = 0;
	static const int IND_TwoPlayer = 1;
	static const int IND_VolumeDown = 2;
	static const int IND_VolumeUp = 3;
	static const int IND_VolumeDisplay = 4;
	static const int IND_KeyBindings = 5;
	static const int IND_HighScores = 6;
	static const int IND_Exit = 7;

	MainMenu();
};
