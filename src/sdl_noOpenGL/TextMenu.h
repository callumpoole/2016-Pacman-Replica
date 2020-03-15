#pragma once
#include "Menu.h"
#include "TextBox.h"
#include "DispText.h"

class TextMenu : public Menu
{
public:
	static const int IND_Words = 0;
	static const int IND_TextBox = 1;
	static const int IND_Submit = 2;

	TextMenu();
	TextMenu(std::string words);

	inline std::string GetString() {
		return dynamic_cast<TextBox*>(widgets[IND_TextBox].get())->text->GetText();
	}
};

