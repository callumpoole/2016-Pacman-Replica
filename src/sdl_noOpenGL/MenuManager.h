#pragma once
#include "Common.h"

class Menu;

class MenuManager
{
public:
	//Not an enum to avoid casting and give scope
	static const int IND_MainMenu = 0;
	static const int IND_PauseMenu = 1;
	static const int IND_KeyMenu = 2;
	static const int IND_HighScoreMenu = 3;
	static const int IND_EnterNameMenu = 4;

	static MenuManager* curManager;
	bool shouldCloseGame = false;
	int setPauseState = -1;
	
	std::vector<std::unique_ptr<Menu>> menus;

	void UpdateVolumeValue();

	MenuManager();

	void Init();

	void Update();
	void Render();
	void MouseMove(Coord c);
	void MouseUp(Coord c);
	void MouseDown(Coord c);
	void KeyDown(SDL_Keycode code);

	void Cleanup();
};


