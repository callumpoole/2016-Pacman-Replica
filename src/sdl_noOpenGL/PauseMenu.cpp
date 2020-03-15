#include "PauseMenu.h"
#include "Button.h"
#include "AudioGenerator.h"
#include "MenuManager.h"
#include "SceneManager.h"
#include "KeyBindingMenu.h"

void Resume(int widgetIndex) {
	MenuManager::curManager->menus[MenuManager::IND_PauseMenu]->HideMenu();
	MenuManager::curManager->setPauseState = 0;
	SceneManager::currentScene->SetAllEntitiesFrozenMovement(false);
}


PauseMenu::PauseMenu() {
	menuRect = SDL_Rect{ 10,10,14,13 };

	std::unique_ptr<Button> btn1(new Button("Resume", SDL_Rect{ 11,11,12,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), &Resume));
	widgets.push_back(std::move(btn1));

	std::unique_ptr<Button> btn2(new Button(" - ", SDL_Rect{ 11,14,2,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {AudioGenerator::IncreasePsuedoVolumeBy(-1); }));
	widgets.push_back(std::move(btn2));

	std::unique_ptr<Button> btn3(new Button(" + ", SDL_Rect{ 21,14,2,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {AudioGenerator::IncreasePsuedoVolumeBy(1); }));
	widgets.push_back(std::move(btn3));

	std::unique_ptr<Button> btn4(new Button("Volume: 10", SDL_Rect{ 14,14,6,2 }, glm::vec3(0, 0, 255), glm::vec3(0, 0, 255)));
	widgets.push_back(std::move(btn4));

	std::unique_ptr<Button> btn5(new Button("Key Bindings", SDL_Rect{ 11,17,12,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {
		MenuManager::curManager->menus[MenuManager::IND_PauseMenu]->HideMenu();
		MenuManager::curManager->menus[MenuManager::IND_KeyMenu]->ShowMenu();
		dynamic_cast<KeyBindingMenu*>(MenuManager::curManager->menus[MenuManager::IND_KeyMenu].get())->previousMenuID = 1;
	}));
	widgets.push_back(std::move(btn5));

	std::unique_ptr<Button> btn6(new Button("Exit", SDL_Rect{ 11,20,12,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {MenuManager::curManager->shouldCloseGame = true; }));
	widgets.push_back(std::move(btn6));
}

