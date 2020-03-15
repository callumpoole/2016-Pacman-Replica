#pragma once
#include "Common.h"
#include "Entity.h"
class Ghost : public Entity
{
private:
	static std::vector<Ghost*> ghosts;
	static int NumGhosts;

	bool isDead = false;
	bool isDeadAndFlashing = false;
	int deadModeTimer = 0;
	static const int DEAD_MODE_DURATION;
	static const float SPEED;
	static const float SLOW_SPEED;
	static const float FAST_SPEED;
	static const float PERCENT_FLASHING; //The percent of the time of dead mode that is spent flashing
	float speed = SPEED;
	Coord gridCoord;

	//Linked to leaving the spawn
	bool inSpawn = true;
	bool isExitingSpawn = false;
	bool isRunningBackToSpawn = false;
	int currentBouncesInSpawn = 0;
	int targetBounces = 20;

	glm::vec2 oldGridPos;

	EDirection Direction = EDirection::Up;

public:
	static int killStreak;
	enum class EGhostType {
		None = 0,
		///Red Ghost
		Blinky = 1,
		///Pink Ghost
		Pinky = 2,
		///Cyan Ghost
		Inky = 3,
		///Orange Ghost
		Clyde = 4
	};
	Ghost();
	Ghost(glm::vec2 pos, EGhostType type);
	EGhostType type;

	void SetDirection(EDirection Direction, bool setVel);
	inline EDirection GetDirection() const { return this->Direction; }
	static void SetAllDeadModes(bool mode);
	void SetDeadMode(bool mode);
	inline bool GetDeadMode() { return isDead; }
	inline bool GetIsRunningBackToSpawn() { return isRunningBackToSpawn; }
	
	void Kill();
	void Reset() override;
	static void HideAllGhosts();

	void MoveInSpawn();
	void CheckIfMove();
	bool CanMoveInDirection(bool incBarrriers = false);

	void Update() override;
	bool GonePastHalfWayOfTile();
};

