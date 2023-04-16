#include "Button.h"
#include "Sprite.h"
#include "Menu.h"
#include "DispText.h"

Button::Button()
{
}
Button::Button(std::string text, SDL_Rect rect, glm::vec3 normCol, glm::vec3 hoverCol, void(clickFunc)(int)) {
	this->rect = rect;
	this->priCol = normCol;
	this->secCol = hoverCol;
	this->clickFunc = clickFunc;
	this->text = new DispText(rect, text, 100, SDL_Color{ 255,255,255 });
}
Button::~Button() {
	delete text;
}


void Button::Render() {
	if (!isVisible)
		return;
	if (isHovering)
		SDL_SetRenderDrawColor(Menu::renderer, (Uint8)priCol.r, (Uint8)priCol.g, (Uint8)priCol.b, 255);
	else
		SDL_SetRenderDrawColor(Menu::renderer, (Uint8)secCol.r, (Uint8)secCol.g, (Uint8)secCol.b, 255);

	SDL_Rect dest = {
		Sprite::topLeftOffset.x + rect.x * Sprite::outputSize,
		Sprite::topLeftOffset.y + rect.y * Sprite::outputSize,
		rect.w * Sprite::outputSize, rect.h * Sprite::outputSize
	};
	SDL_RenderFillRect(Menu::renderer, &dest);

	text->Render();
}