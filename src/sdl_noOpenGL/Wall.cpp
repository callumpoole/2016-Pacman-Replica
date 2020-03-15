#include "Wall.h"
#include "Sprite.h"
#include "Animation.h"
#include "Ghost.h"

bool Wall::isFlashing = false;
bool Wall::hasFlashed = false;
int Wall::wallCount = 0;

// Indexed with the EWallType enum - 1
Coord Wall::wallSpriteCoords[7] = {
	{ 4,4 }, //vertical
	{ 4,3 }, //horizontal
	{ 5,3 }, //topLeft
	{ 6,3 }, //topRight
	{ 5,4 }, //bottomLeft
	{ 6,4 }, //bottomRight
	{ 6,7 }, //ghostBarrier
};


Wall::Wall(glm::vec2 pos, EWallType wallType) : Entity(pos, EEntityType::wall), wallType(wallType) {
	wallID = wallCount++;
	componentManager.AddComponent<Sprite>(&this->pos)->SetSpriteCoord(wallSpriteCoords[(int)wallType - 1]);
	if (wallType == EWallType::ghostBarrier)
		ghostWall = true;
}

void Wall::DoFlashingAnimation() {
	SceneManager::currentScene->SetAllEntitiesFrozenMovement(true);
	isFlashing = true;
	hasFlashed = false;

	//Hide Ghosts
	Ghost::HideAllGhosts();

	//Finish Player Animation
	for (std::shared_ptr<Entity> e : SceneManager::currentScene->GetEntitiesOfType(EEntityType::player))
		e->componentManager.GetComponent<Animation>()->SetAnim(0, false, false);

	for (std::shared_ptr<Entity> e : SceneManager::currentScene->GetEntitiesOfType(EEntityType::wall)) {
		Sprite* s = e->componentManager.GetComponent<Sprite>();
		Coord c = s->GetSpriteCoord();
		if (c.y != int(EWallType::ghostBarrier)) //Not a ghost barrier
			s->SetSpriteCoord(c.x, c.y + 2);
	}
}

void Wall::Update() {
	Entity::Update();

	if (isFlashing && wallType != EWallType::ghostBarrier) {
		if (curFlashTicks++ == TOTAL_FLASH_TICKS) {
			curFlashTicks = 0;
			hasFlashed = !hasFlashed;
			Sprite* s = componentManager.GetComponent<Sprite>();
			Coord c = s->GetSpriteCoord();
			if (c.y < 5)
				s->SetSpriteCoord(c.x, c.y + 2);
			else if (c.y > 4)
				s->SetSpriteCoord(c.x, c.y - 2);

			if (curFlashCount++ == TOTAL_FLASH_COUNT) {
				hasFlashed = false;
				curFlashCount = 0;
				if (c.y > 4)
					s->SetSpriteCoord(c.x, c.y - 2);
				if (wallID == wallCount - 1)
					isFlashing = false;
				for (std::shared_ptr<Entity> e : SceneManager::currentScene->GetEntitiesOfType(EEntityType::player))
					e->SetVisibility(false);
				SceneManager::currentScene->RestartLevel(SceneManager::currentScene->GetLevel() + 1);
			}
		}
	}
}