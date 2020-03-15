#include "MainMenu.h"
#include "MenuManager.h"
#include "AudioGenerator.h"
#include "SceneManager.h"
#include "Button.h"
#include "KeyBindingMenu.h"

void SelectOnePlayer(int widgetIndex) {
	SceneManager::currentScene->SetAllEntitiesFrozenMovement(false);

	//Disable the second player
	SceneManager::currentScene->GetEntitiesOfType(Entity::EEntityType::player)[1]->SetAlive(false); 

	MenuManager::curManager->menus[MenuManager::IND_MainMenu]->HideMenu();
}
void SelectTwoPlayer(int widgetIndex) {
	SceneManager::currentScene->SetAllEntitiesFrozenMovement(false);

	MenuManager::curManager->menus[MenuManager::IND_MainMenu]->HideMenu();
}

MainMenu::MainMenu() {
	menuRect = SDL_Rect{10,9,14,19 };

	std::unique_ptr<Button> btn1(new Button("One Player", SDL_Rect{ 11,10,12,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), &SelectOnePlayer));
	widgets.push_back(std::move(btn1));

	std::unique_ptr<Button> btn2(new Button("Two Player", SDL_Rect{ 11,13,12,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255) , &SelectTwoPlayer));
	widgets.push_back(std::move(btn2));

	std::unique_ptr<Button> btn3(new Button(" - ", SDL_Rect{ 11,16,2,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {AudioGenerator::IncreasePsuedoVolumeBy(-1); })); //, &VolumeDown));
	widgets.push_back(std::move(btn3));

	std::unique_ptr<Button> btn4(new Button(" + ", SDL_Rect{ 21,16,2,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {AudioGenerator::IncreasePsuedoVolumeBy(1); }));    // , &VolumeUp));
	widgets.push_back(std::move(btn4));

	std::unique_ptr<Button> btn5(new Button("Volume: 10", SDL_Rect{ 14,16,6,2 }, glm::vec3(0, 0, 255), glm::vec3(0, 0, 255)));
	widgets.push_back(std::move(btn5));

	std::unique_ptr<Button> btn6(new Button("Key Bindings", SDL_Rect{ 11,19,12,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {
		MenuManager::curManager->menus[MenuManager::IND_MainMenu]->HideMenu();
		MenuManager::curManager->menus[MenuManager::IND_KeyMenu]->ShowMenu();
		dynamic_cast<KeyBindingMenu*>(MenuManager::curManager->menus[MenuManager::IND_KeyMenu].get())->previousMenuID = 0;
	}));
	widgets.push_back(std::move(btn6));

	std::unique_ptr<Button> btn7(new Button("High Scores", SDL_Rect{ 11,22,12,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {
		MenuManager::curManager->menus[MenuManager::IND_MainMenu]->HideMenu();
		MenuManager::curManager->menus[MenuManager::IND_HighScoreMenu]->ShowMenu();
	}));
	widgets.push_back(std::move(btn7));

	std::unique_ptr<Button> btn8(new Button("Exit", SDL_Rect{ 11,25,12,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {MenuManager::curManager->shouldCloseGame = true; }));   //, &Exit)); 
	widgets.push_back(std::move(btn8));
}

