#include "MenuManager.h"
#include "Button.h"
#include "AudioGenerator.h"
#include "HighScoresMenu.h"
#include "TextMenu.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "KeyBindingMenu.h"

MenuManager* MenuManager::curManager;

void MenuManager::UpdateVolumeValue() { 
	Button* btn;
	if (btn = dynamic_cast<Button*>(menus[IND_MainMenu]->widgets[MainMenu::IND_VolumeDisplay].get()))
		btn->text->SetText("Volume: " + std::to_string(AudioGenerator::GetPsuedoVolume()));
	if (btn = dynamic_cast<Button*>(menus[IND_PauseMenu]->widgets[PauseMenu::IND_VolumeDisplay].get()))
		btn->text->SetText("Volume: " + std::to_string(AudioGenerator::GetPsuedoVolume()));
}

MenuManager::MenuManager() {
	curManager = this;
}

void MenuManager::Init()
{
	std::unique_ptr<Menu> mainMenu(new MainMenu());
	menus.push_back(std::move(mainMenu));

	std::unique_ptr<Menu> pauseMenu(new PauseMenu());
	menus.push_back(std::move(pauseMenu));

	std::unique_ptr<Menu> keyMenu(new KeyBindingMenu());
	menus.push_back(std::move(keyMenu));

	std::unique_ptr<Menu> highScoreMenu(new HighScoresMenu());
	menus.push_back(std::move(highScoreMenu));

	std::unique_ptr<Menu> enterNameMenu(new TextMenu(std::string{ "Enter your name(s):" }));
	menus.push_back(std::move(enterNameMenu));
}

void MenuManager::Update() {
	for (auto& ob : menus)
		ob->Update();
}

void MenuManager::Render() {
	for (auto& ob : menus)
		ob->Render();
}

void MenuManager::MouseMove(Coord c) {
	for (auto& ob : menus)
		ob->MouseMove(c);
}

void MenuManager::MouseUp(Coord c) {
	for (auto& ob : menus)
		if (ob->MouseUp(c))
			return;
}

void MenuManager::MouseDown(Coord c) {
	for (auto& ob : menus)
		if (ob->MouseDown(c))
			return;
}

void MenuManager::KeyDown(SDL_Keycode code) {
	for (auto& ob : menus)
		ob->GetKeyDown(code);
}

void MenuManager::Cleanup() {
	for (auto& ob : menus)
		ob->Cleanup();
}
