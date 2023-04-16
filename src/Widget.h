#pragma once
#include "Common.h"
//#include "Menu.h"
#include "Sprite.h"
class Widget
{
protected:
	void(*clickFunc)(int);

	SDL_Rect rect;
	glm::vec3 priCol;
	glm::vec3 secCol;
	bool isHovering = false;
public:
	bool isVisible = true;
	Widget() {}

	virtual void Render() {}
	virtual void Update() {}
	virtual bool MouseDown(Coord c, int widgetIndex = -1);
	virtual bool MouseMove(Coord c);
	virtual bool MouseUp(Coord c);
	virtual void GetKeyDown(SDL_Keycode code) { }
};

