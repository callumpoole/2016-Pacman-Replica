#pragma once
#include "Menu.h"
class KeyBindingMenu : public Menu
{
	std::string KeyBindingMenu::SDLKCodeToLetter(SDL_Keycode code);
public:
	//Not an enum to avoid casting and give scope
	static const int IND_Player1 = 0;
	static const int IND_Player2 = 1;
	static const int IND_W = 2;
	static const int IND_A = 3;
	static const int IND_S = 4;
	static const int IND_D = 5;
	static const int IND_Up = 6;
	static const int IND_Left = 7;
	static const int IND_Down = 8;
	static const int IND_Right = 9;
	static const int IND_F = 10;
	static const int IND_Fullscreen = 11;
	static const int IND_PressAnyKey = 12;
	static const int IND_Done = 13;

	static int previousMenuID;
	static int lineIDToWrite;

	KeyBindingMenu();

	void UpdateKeyButtons();

	void KeyBindingMenu::HidePressKeyMessage();

	void KeyBindingMenu::ShowMenu() override;
};

