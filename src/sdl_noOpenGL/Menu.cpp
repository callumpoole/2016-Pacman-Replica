#include "Menu.h"
#include "Sprite.h"

SDL_Renderer *Menu::renderer; //pointer to the renderer from main, link made in: SetupSprites()

void Menu::Update() {
	if (isVisible)
		for (const auto& w : widgets)
			w->Update();
}
void Menu::Render() {
	if (isVisible) {
		SDL_SetRenderDrawColor(renderer, (Uint8)menuCol.r, (Uint8)menuCol.g, (Uint8)menuCol.b, 255);

		SDL_Rect dest = { Sprite::topLeftOffset.x + menuRect.x * Sprite::outputSize,
						  Sprite::topLeftOffset.y + menuRect.y * Sprite::outputSize,
						  menuRect.w * Sprite::outputSize, menuRect.h * Sprite::outputSize };


		SDL_RenderFillRect(renderer, &dest);

		for (const auto& w : widgets)
			w->Render();
	}
}
bool Menu::MouseDown(Coord c) {
	int widgetIndex = 0;
	if (isVisible)
		for (const auto& w : widgets) {
			if (w->MouseDown(c, widgetIndex)) //Stops one click from pressing many buttons
				return true;
			widgetIndex++;
		}
	return false;
}
void Menu::MouseMove(Coord c) {
	if (isVisible)
		for (const auto& w : widgets)
			w->MouseMove(c);
}
bool Menu::MouseUp(Coord c) {
	if (isVisible)
		for (const auto& w : widgets)
			if (w->MouseUp(c))
				return true;
	return false;
}
void Menu::GetKeyDown(SDL_Keycode code) {
	if (isVisible)
		for (const auto& w : widgets)
			w->GetKeyDown(code);
}