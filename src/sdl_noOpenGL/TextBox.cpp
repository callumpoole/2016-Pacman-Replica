#include "TextBox.h"
#include "Menu.h"
#include "DispText.h"

TextBox::TextBox()
{
}

TextBox::TextBox(std::string text, SDL_Rect rect, glm::vec3 normCol, glm::vec3 hoverCol, void(clickFunc)(int)) {
	this->rect = rect;
	this->priCol = normCol;
	this->secCol = hoverCol;
	this->clickFunc = clickFunc;
	this->text = new DispText(rect, text, 100, SDL_Color{ 255,255,255 });
	this->text->autoWidth = 1;
}
TextBox::~TextBox() {
	delete text;
}

void TextBox::GetKeyDown(SDL_Keycode code) {
	if (isVisible) {
		if (code != 8) {
			if (code >= 'a' && code <= 'z') //Capitals
				code = code - 32;
			text->SetText(text->GetText() + (char)code);
		}
		else
			text->SetText(text->GetText().substr(0, text->GetText().size() - 1));
	}

}

void TextBox::Render() {
	if (!isVisible)
		return;
	SDL_SetRenderDrawColor(Menu::renderer, (Uint8)priCol.r, (Uint8)priCol.g, (Uint8)priCol.b, 255);

	SDL_Rect dest = {
		Sprite::topLeftOffset.x + rect.x * Sprite::outputSize,
		Sprite::topLeftOffset.y + rect.y * Sprite::outputSize,
		rect.w * Sprite::outputSize, rect.h * Sprite::outputSize
	};
	SDL_RenderFillRect(Menu::renderer, &dest);

	text->Render();
}
