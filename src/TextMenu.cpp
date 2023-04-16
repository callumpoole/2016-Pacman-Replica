#include "TextMenu.h"
#include "Button.h"
#include "TextBox.h"
#include "HighScoresMenu.h"
#include "MenuManager.h"
#include "Collectable.h"

TextMenu::TextMenu()
{
}

TextMenu::TextMenu(std::string words = std::string{"Enter some text: "}) {
	menuRect = SDL_Rect{ 10,9,14,12 };

	std::unique_ptr<Button> btn1(new Button(words, SDL_Rect{ 11,10,12,2 }, glm::vec3(0, 0, 255), glm::vec3(0, 0, 255)));
	widgets.push_back(std::move(btn1));

	std::unique_ptr<TextBox> txt(new TextBox("", SDL_Rect{ 11,13,12,2 }, glm::vec3(0, 0, 255), glm::vec3(0, 0, 255)));
	widgets.push_back(std::move(txt));

	std::unique_ptr<Button> btnS(new Button("Submit", SDL_Rect{ 11,16,12,2 }, glm::vec3(0, 255, 255), glm::vec3(0, 0, 255), [](int) {
		HighScoresMenu* hsm = dynamic_cast<HighScoresMenu*>(MenuManager::curManager->menus[MenuManager::IND_HighScoreMenu].get());
		TextMenu* txtM = dynamic_cast<TextMenu*>(MenuManager::curManager->menus[MenuManager::IND_EnterNameMenu].get());
		std::string text = txtM->GetString();
		hsm->AddScore(text, Collectable::GetScore());
		txtM->HideMenu();
	}));
	widgets.push_back(std::move(btnS));
}

