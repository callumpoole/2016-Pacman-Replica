#include "HealthBar.h"
#include "Sprite.h"

HealthBar* HealthBar::currentHealthBar = nullptr;
int HealthBar::lives = STARTING_LIVES;

HealthBar::HealthBar(glm::vec2 pos) : Entity(pos, EEntityType::other) {
	currentHealthBar = this;
	SetRenderingLayer(3);
	for (int i = 0; i < MAX_LIVES; i++) {
		Sprite* s = componentManager.AddComponent<Sprite>(&this->pos);
		s->SetSpriteCoord(Coord{ 3, 1 });
		s->flip = SDL_FLIP_HORIZONTAL;
		s->offsetFromPos = glm::vec2(-i, 0); //From right to left
	}
	ResetLives();
}

void HealthBar::SetLives(int x) {
	lives = x;
	auto sprite = componentManager.GetComponents<Sprite>();
	for (int i = 0; i < MAX_LIVES; i++)
		(*sprite)[i]->isVisible = i < lives + (SHOW_CURRENT_LIFE ? 0 : -1);
}