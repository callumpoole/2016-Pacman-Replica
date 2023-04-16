#pragma once
#include "Menu.h"
#include "DispText.h"
class HighScoresMenu : public Menu
{
	std::vector<std::unique_ptr<DispText>> scores;

	struct HighScore {
		HighScore() {}
		HighScore(std::string name, int score) {
			this->name = name;
			this->score = score;
		}

		std::string name = "";
		int score = 0;
	};

	void SortScores(std::vector<HighScore>& items);
public:
	static const int IND_HighScores = 0;
	static const int IND_Back = 1;

	HighScoresMenu();

	void LoadScores();

	void Render() override;

	void AddScore(std::string name, int score);
};

