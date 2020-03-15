#pragma once
#include "Widget.h"

class DispText;

class Button : public Widget
{
public:
	DispText* text;

	Button();
	Button(std::string text, SDL_Rect rect, glm::vec3 normCol, glm::vec3 hoverCol, void(clickFunc)(int) = [](int) {});
	~Button();

	void Render() override;
};
