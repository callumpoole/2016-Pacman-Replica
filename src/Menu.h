#pragma once
#include "Common.h"
#include "Widget.h"
class Menu
{
protected:
	SDL_Rect menuRect = SDL_Rect{ 10,10,14,10 };
	glm::vec3 menuCol = { 255,0,0 };

	bool isVisible = false;
public:
	std::vector<std::unique_ptr<Widget>> widgets;
	static SDL_Renderer *renderer; //pointer to the renderer from main, link made in: SetupSprites()

	Menu() {}

	static inline void StaticMenuSetup(SDL_Renderer* renderer) { Menu::renderer = renderer; }
	static inline void StaticMenuCleanup() {}

	virtual inline void ShowMenu() { isVisible = true; }
	virtual inline void HideMenu() { isVisible = false; }
	virtual inline bool GetVisibility() { return isVisible; }

	virtual void Update();
	virtual void Render();
	virtual bool MouseDown(Coord c);
	virtual void MouseMove(Coord c);
	virtual bool MouseUp(Coord c);
	virtual void GetKeyDown(SDL_Keycode code);
	virtual inline void Cleanup() {}
};
