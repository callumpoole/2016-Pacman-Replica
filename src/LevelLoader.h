#pragma once
#include "Entity.h"
class LevelLoader
{
private:
	static const char* LEVELS_PATH;
	static int gridSize;
public:
	enum class EEntityType : int {
		empty = 0,
		vertical = 1,
		horizontal = 2,
		topLeft = 3,
		topRight = 4,
		bottomLeft = 5,
		bottomRight = 6,
		ghostBarrier = 7,
		player1Spawn = 8,
		player2Spawn = 9,
		ghostSpawn = 10,
		dot = 11,
		bigDot = 12,
		fruit = 13,
		teleporter = 14
	};

	static EEntityType** grid;
	static void LoadFile(const char* filename);
	static inline void Unload() { delete[] grid; }

	static void PlaceAllTiles(std::vector<std::shared_ptr<Entity>>& sceneObjs, bool onlyDots = false);

	static bool IsWall(int x, int y, bool incBarrier = true);

	static inline bool IsWall(EEntityType e, bool incBarrier) 
	{ return e >= EEntityType::vertical && int(e) <= int(EEntityType::bottomRight) + incBarrier ? 1 : 0; }

	///Returns: the amount of neighbouring EEntityType::empty spaces, 0 if the block isn't an EEntityType::empty space
	static int IsJunction(int x, int y, EDirection dirs[4] = nullptr);
};

