#include "HighScoresMenu.h"
#include <sstream>
#include <string>
#include <fstream>
#include "Button.h"
#include "MenuManager.h"


HighScoresMenu::HighScoresMenu()
{
	menuRect = SDL_Rect{ 8,9,18,19 };

	std::unique_ptr<Button> btn1(new Button("High Scores", SDL_Rect{ 11,10,12,2 }, glm::vec3(0, 0, 255), glm::vec3(0, 0, 255)));
	widgets.push_back(std::move(btn1));

	std::unique_ptr<Button> btnBack(new Button("Back", SDL_Rect{ 11,25,12,2 }, glm::vec3(0, 255, 0), glm::vec3(0, 0, 255), [](int) {
		MenuManager::curManager->menus[MenuManager::IND_MainMenu]->ShowMenu();
		MenuManager::curManager->menus[MenuManager::IND_HighScoreMenu]->HideMenu();
	}));
	widgets.push_back(std::move(btnBack));

	LoadScores();
}

void HighScoresMenu::LoadScores() {

	std::ifstream infile("Resources\\HighScores.txt");

	std::string line; 
	for (int i = 0;std::getline(infile, line) && i < 10; i++) { 
		std::istringstream ss(line);
		std::string token; 
		for (int j = 0; std::getline(ss, token, ','); j++) {
				std::unique_ptr<DispText> ptr(new DispText(SDL_Rect{ 9 + j * 13, 13 + i ,4,1 }, std::string(token)));
				ptr->autoWidth = 1;
				scores.push_back(std::move(ptr));
		} 
	}
	infile.close();
}


void HighScoresMenu::Render()
{
	if (!isVisible)
		return;
	Menu::Render();
	for (auto& s : scores) {
		s->Render();
	}
}

void HighScoresMenu::SortScores(std::vector<HighScore>& items) {
	bool swapMade = true;
	while (!swapMade) {
		swapMade = false;
		for (unsigned int i = 0; i < items.size()-1; i++) {
			if (items[i].score > items[i + 1].score) {
				HighScore temp = items[i + 1];
				items[i + 1] = items[i];
				items[i] = temp;
				swapMade = true;
			}
		}
	}
}

void HighScoresMenu::AddScore(std::string name, int score)
{
	std::ifstream infile("Resources\\HighScores.txt");
	std::vector<HighScore> items;
	items.push_back(HighScore{ name, score });

	std::string line;
	for (int i = 0; std::getline(infile, line); i++) { 
		std::istringstream ss(line);
		std::string token;
		HighScore it;
		for (int j = 0; std::getline(ss, token, ','); j++) 
			if (j == 0) 
				it.name = token; 
			else 
				it.score = std::stoi(token); 
		items.push_back(it);
	}
	SortScores(items);
	infile.close();

	std::ofstream a_file("Resources\\HighScores.txt");
	for (unsigned int i = 0; i < items.size(); i++)
		a_file << items[i].name << "," << items[i].score << std::endl;
	a_file.close();
}




/*

std::shared_ptr<DispText> ptr2(new DispText(SDL_Rect{ 6,33,4,1 }, std::string("0")));
ptr2->autoWidth = 1;
ptr2->tag = "ScoreCount";
scene.textObjs.push_back(std::move(ptr2));

*/