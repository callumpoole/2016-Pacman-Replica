#pragma once
#include "Entity.h"

class HealthBar : public Entity {
private:
	static const bool SHOW_CURRENT_LIFE = false;
	static const int MAX_LIVES = 10;
	static const int STARTING_LIVES = 3;
	static int lives;
public:
	static HealthBar* currentHealthBar;
	HealthBar() {}
	HealthBar(glm::vec2 pos);

	void SetLives(int x);
	inline void ResetLives() { SetLives(STARTING_LIVES); }
	inline int GetLives() { return lives; }
	inline int AddLive() { if (lives < MAX_LIVES) SetLives(++lives); return lives; }
	inline int RemoveLive() { if (lives > 0) SetLives(--lives); return lives; }
	inline bool IsDead() { return lives <= 0; }
};
