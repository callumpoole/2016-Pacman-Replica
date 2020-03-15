#pragma once
#include "Widget.h"

class DispText;

class TextBox : public Widget
{
public:
	DispText* text;

	TextBox();
	TextBox(std::string text, SDL_Rect rect, glm::vec3 normCol, glm::vec3 hoverCol, void(clickFunc)(int) = [](int) {});
	~TextBox();

	void GetKeyDown(SDL_Keycode code) override;

	void Render() override;
};

