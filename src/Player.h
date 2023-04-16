#pragma once
#include "Entity.h"

class Player : public Entity
{
private:
	static std::vector<Player*> players;
	static bool respawnStage1;

	static const float SPEED;
	EDirection currentDir;
	EDirection chosenDir;
	Coord gridCoord;

	int respawnTimer = 0;
	const int RESPAWN_TICKS = 300;
	
	bool player1 = true;
	bool localPlayer = true; //Networking

	const int TICKS_FOR_MUSIC_CHANGE = 100;
public:
	int ticksSinceLastDot = 0;
	bool isDying = false;

	Player();
	Player(glm::vec2 pos, bool player1);

	static const char* PACMAN_ANIM_FILE;
	static const char* MRS_PACMAN_ANIM_FILE;

	void Kill();
	void Respawn();

	void SelectDirection(EDirection direction);
	void CheckIfMove();
	void MoveDirection(EDirection direction);

	void Update() override;
};

