#pragma once
#include "Entity.h"
#include "SceneManager.h"

enum class EWallType : int {
	empty = 0,
	vertical = 1,
	horizontal = 2,
	topLeft = 3,
	topRight = 4,
	bottomLeft = 5,
	bottomRight = 6,
	ghostBarrier = 7,
};

class Wall : public Entity {
private:
	static int wallCount;
	int wallID = 0;

	static bool isFlashing;
	static bool hasFlashed;
	int curFlashTicks = 0;
	const int TOTAL_FLASH_TICKS = 30;
	int curFlashCount = 0;
	const int TOTAL_FLASH_COUNT = 4;
public:
	EWallType wallType;
	static Coord wallSpriteCoords[7]; //The lighter version can be accessed by adding 2 to each Y (except for the ghost wall (loop 0 to 5))
	bool ghostWall = false;

	Wall(glm::vec2 pos, EWallType wallType);

	static void DoFlashingAnimation();

	void Update() override;
};
