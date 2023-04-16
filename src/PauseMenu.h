#pragma once
#include "Menu.h"
class PauseMenu : public Menu
{
public:
	static const int IND_Resume = 0;
	static const int IND_VolumeDown = 1;
	static const int IND_VolumeUp = 2;
	static const int IND_VolumeDisplay = 3;
	static const int IND_KeyBindings = 4;
	static const int IND_Exit = 5;

	PauseMenu();
};

