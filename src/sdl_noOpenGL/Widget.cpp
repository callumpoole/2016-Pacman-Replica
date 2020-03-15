#include "Widget.h"

bool Widget::MouseDown(Coord c, int widgetIndex) {
	SDL_Rect dest = { Sprite::topLeftOffset.x + rect.x * Sprite::outputSize,
					  Sprite::topLeftOffset.y + rect.y * Sprite::outputSize,
					  rect.w * Sprite::outputSize, rect.h * Sprite::outputSize };

	if (c.x >= dest.x && c.x <= dest.x + dest.w && c.y >= dest.y && c.y <= dest.y + dest.h) {
		clickFunc(widgetIndex);
		return true;
	}
	return false;
}
bool Widget::MouseMove(Coord c) {
	SDL_Rect dest = {
		Sprite::topLeftOffset.x + rect.x * Sprite::outputSize,
		Sprite::topLeftOffset.y + rect.y * Sprite::outputSize,
		rect.w * Sprite::outputSize, rect.h * Sprite::outputSize
	};

	isHovering = c.x >= dest.x && c.x <= dest.x + dest.w && c.y >= dest.y && c.y <= dest.y + dest.h;

	return isHovering;
}
bool Widget::MouseUp(Coord c) {
	MouseMove(c);
	return isHovering;
}