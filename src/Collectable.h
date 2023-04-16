#pragma once
#include <algorithm>
#include "Entity.h"

class Collectable : public Entity
{
public:
	enum class ECollectable {
		None = 0,
		smallDot = 1,
		largeDot = 2,
		fruit = 3
	};
private:
	int timeToSpawnCounter = 0;
	bool hasSpawned = true;
	const int timeToSpawn = 1000; //Ticks
	const int timeToDespawn = 2000; //Ticks

	static int score;
	static int dotsToCollect;
	static int bigDotsToCollect;
public:
	static Collectable* curFruit;
	ECollectable collectableType;

	Collectable(glm::vec2 pos, ECollectable collectableType);

	inline bool HasSpawned() { return hasSpawned; }
	static inline int Collectable::GetScore() { return score; }
	static void AddToScore(int points);
	static void ResetScore();

	void Collect();
	void Reset() override;
	void Update() override;
};

