#include "InputController.h"
#include <sstream>
#include <string>
#include <fstream>
#include "Common.h"
#include "Entity.h"
#include "Player.h"
#include "SceneManager.h"

SDL_Keycode InputController::fullScreen = SDLK_f;

void InputController::GetKeyDown(SDL_Keycode key)
{
	if      (key == up)    dynamic_cast<Player*>(entity)->SelectDirection(EDirection::Up);
	else if (key == down)  dynamic_cast<Player*>(entity)->SelectDirection(EDirection::Down);
	else if (key == left)  dynamic_cast<Player*>(entity)->SelectDirection(EDirection::Left);
	else if (key == right) dynamic_cast<Player*>(entity)->SelectDirection(EDirection::Right);
}

InputController::InputController(int playerID) {
	type = EComponentType::InputController;
	LoadKeyBindings(playerID);
}

bool InputController::LoadKeyBindings(int playerID) {
	try {
		std::ifstream infile(KEYBINDINGS_PATH);
		
		std::cout << "Loading Controls for P" << playerID << std::endl;

		std::string line;
		for (int i = 0; std::getline(infile, line); i++) {
			if (((playerID == 0 && i < 4) || (playerID == 1 && i >= 4)) && i < 8) {
				if (i % 4 == 0) 
					up = (SDL_Keycode)std::stoi(line);
				else if (i % 4 == 1) 
					left = (SDL_Keycode)std::stoi(line);
				else if (i % 4 == 2) 
					down = (SDL_Keycode)std::stoi(line);
				else if (i % 4 == 3) 
					right = (SDL_Keycode)std::stoi(line);
			}
		}
		infile.close();
		return true;
	}catch(...) {
		std::cout << "Error loading: " KEYBINDINGS_PATH "\n" \
					 "Setting defaults to: W A S D  |  EDirection::Up EDirection::Left EDirection::Down EDirection::Right\n";
		return false;
	}
}



bool InputController::SetKeyBinding(int indexInFile, SDL_Keycode code) {
	std::ifstream filein(KEYBINDINGS_PATH); //File to read from

	std::string lines[9];
	std::string line;
	for (int i = 0; std::getline(filein, line); i++) {
		lines[i] = line;
	}
	filein.close();
	
	std::ofstream fileout(KEYBINDINGS_PATH);

	for (int i = 0; i < 9; i++) {
		if (i == indexInFile)
			fileout << code << "\n";
		else
			fileout << lines[i] << "\n";
	}

	fileout.close();

	for (int i = 0; i < 2; i++)
		SceneManager::currentScene->GetEntitiesOfType(Entity::EEntityType::player)[i]->
			componentManager.GetComponent<InputController>()->LoadKeyBindings(i);

	return true;
}
