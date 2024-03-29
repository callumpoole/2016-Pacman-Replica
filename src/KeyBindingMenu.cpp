#include "KeyBindingMenu.h"
#include <fstream>
#include <locale>
#include "MenuManager.h"
#include "Button.h"
#include "InputController.h"
#include "DispText.h"

int KeyBindingMenu::previousMenuID = -1;
int KeyBindingMenu::lineIDToWrite = -1;

void KeyBindPress(int widgetIndex) {
	KeyBindingMenu::lineIDToWrite = widgetIndex - 2;
	MenuManager::curManager->menus[MenuManager::IND_KeyMenu]->widgets[KeyBindingMenu::IND_PressAnyKey]->isVisible = true;
}

KeyBindingMenu::KeyBindingMenu() {
	menuRect = SDL_Rect{ 7,9,20,19 };

	const int offset = 10;

	for (int i = 1; i < 3; i++) {
		std::unique_ptr<Button> P(new Button("Player " + std::to_string(i), SDL_Rect{ 8 + (i-1)*offset,10,8,2 }, glm::vec3(0, 0, 255), glm::vec3(0, 0, 255), [](int) {}));
		widgets.push_back(std::move(P));
	}

	for (int i = 0; i < 2; i++) {  
		std::unique_ptr<Button> Up(new Button("W", SDL_Rect{ 11 + offset * i, 13,2,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), &KeyBindPress));
		widgets.push_back(std::move(Up));
		std::unique_ptr<Button> Left(new Button("A", SDL_Rect{ 8 + offset * i, 16,2,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), &KeyBindPress));
		widgets.push_back(std::move(Left));
		std::unique_ptr<Button> Down(new Button("S", SDL_Rect{ 11 + offset * i, 19,2,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), &KeyBindPress));
		widgets.push_back(std::move(Down));
		std::unique_ptr<Button> Right(new Button("D", SDL_Rect{ 14 + offset * i, 16,2,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), &KeyBindPress));
		widgets.push_back(std::move(Right));
	}

	std::unique_ptr<Button> F(new Button("F", SDL_Rect{ 22,22,2,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), &KeyBindPress));
	widgets.push_back(std::move(F));
	std::unique_ptr<Button> FullScreen(new Button("FullScreen:", SDL_Rect{ 10,22,11,2 }, glm::vec3(0, 0, 255), glm::vec3(0, 0, 255), [](int) {}));
	widgets.push_back(std::move(FullScreen));

	std::unique_ptr<Button> PressKey(new Button("Press the Key Now", SDL_Rect{ 8,6,18,2 }, glm::vec3(0, 0, 255), glm::vec3(0, 0, 255), [](int){} ));
	PressKey->isVisible = false;
	widgets.push_back(std::move(PressKey));

	previousMenuID = -1;

	std::unique_ptr<Button> btnLast(new Button("Done", SDL_Rect{ 12,25,10,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {
		MenuManager::curManager->menus[previousMenuID]->ShowMenu();
		MenuManager::curManager->menus[MenuManager::IND_KeyMenu]->HideMenu();
	}));
	widgets.push_back(std::move(btnLast));
}

void KeyBindingMenu::ShowMenu()
{
	Menu::ShowMenu();
	UpdateKeyButtons();
}

std::string KeyBindingMenu::SDLKCodeToLetter(SDL_Keycode code) {
	std::string output = "";
	
	if (code > 255) {
		if (code == SDLK_UP) output = "Up";
		if (code == SDLK_LEFT) output = "Left";
		if (code == SDLK_DOWN) output = "Down";
		if (code == SDLK_RIGHT) output = "Right";
		if (code >= SDLK_KP_1 && code <= SDLK_KP_9) output = "Num" + std::to_string(code - SDLK_KP_1+1);
		if (code == SDLK_KP_0) output = "Num0";
		if (code >= SDLK_F1 && code <= SDLK_F12) output = "F" + std::to_string(code - SDLK_F1 + 1);
		if (code == SDLK_LCTRL) output = "LCTRL";
		if (code == SDLK_RCTRL) output = "RCTRL";
		if (code == SDLK_LSHIFT) output = "LSHIFT";
		if (code == SDLK_RSHIFT) output = "RSHIFT";
	}
	else {
		char a = static_cast<char>(code);
		if (a >= 'a' && a <= 'z') 
			a = (char)((int)a - 32); //ToUpper 
		output += a;
		if (a == 32) output = "Space";
		if (a == 13) output = "Enter";
	}

	return output;
}

void KeyBindingMenu::HidePressKeyMessage() {
	MenuManager::curManager->menus[MenuManager::IND_KeyMenu]->widgets[IND_PressAnyKey]->isVisible = false;

	UpdateKeyButtons();
}

void KeyBindingMenu::UpdateKeyButtons() {
	std::ifstream filein("..\\src\\Resources\\KeyBindings.txt"); //File to read from
	const int NUM_LINES_IN_FILE = 4 + 4 + 1; //W A S D Up Left Down Right F
	std::string lines[NUM_LINES_IN_FILE]; 
	std::string line;
	for (int i = 0; std::getline(filein, line); i++)
		lines[i] = line;

	filein.close();

	const int FULL_SCREEN_BUTTON_INDEX = 8;
	InputController::fullScreen = (SDL_Keycode)std::stoi(lines[FULL_SCREEN_BUTTON_INDEX]);

	for (int i = 0; i < 9; i++) {
		dynamic_cast<Button*>(MenuManager::curManager->menus[MenuManager::IND_KeyMenu]->widgets[i + IND_W].get())->text->SetText(
			SDLKCodeToLetter(std::stoi(lines[i]))
		);
	}
}