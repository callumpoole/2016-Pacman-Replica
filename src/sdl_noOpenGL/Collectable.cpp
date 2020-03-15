#include "Collectable.h"
#include "CollisionBox.h"
#include "Ghost.h"
#include "AudioGenerator.h"
#include "Wall.h"
#include "Sprite.h"
#include "Animation.h"

int Collectable::dotsToCollect = 0;
int Collectable::bigDotsToCollect = 0;
int Collectable::score = 0;
Collectable* Collectable::curFruit = nullptr;


Collectable::Collectable(glm::vec2 pos, ECollectable collectableType) : Entity(pos, EEntityType::collectable), collectableType(collectableType) {
	SetRenderingLayer(1);
	entityType = Entity::EEntityType::collectable;
	switch (collectableType)
	{
	case ECollectable::None:
		break;
	case ECollectable::smallDot:
		componentManager.AddComponent<Sprite>(&this->pos)->SetSpriteCoord(4, 0);
		dotsToCollect++;
		break;
	case ECollectable::largeDot:
	{
		dotsToCollect++;
		bigDotsToCollect++;
		Sprite* s = componentManager.AddComponent<Sprite>(&this->pos);
		s->SetSpriteCoord(5, 0);

		Animation* a = componentManager.AddComponent<Animation>(s);
		a->LoadFile("big_dot.anim");
		a->SetAnim(0, true, 15);
	}
	break;
	case ECollectable::fruit:
		curFruit = this;
		componentManager.AddComponent<Sprite>(&this->pos)->SetSpriteCoord(std::min(9 + SceneManager::currentScene->GetLevel() - 1, 11), 0); //9,0 Or 10,0 or 11,0 for the different fruits
		hasSpawned = false;
		SetVisibility(false);
		componentManager.AddComponent<AudioGenerator>();
		break;
	default:
		break;
	}
	componentManager.AddComponent<CollisionBox>(this, [](Entity*, CollisionBox*) {});
}

void Collectable::AddToScore(int points) { score += points; }
void Collectable::ResetScore() { score = 0; }

void Collectable::Collect() {
	hasSpawned = false;
	if (collectableType == Collectable::ECollectable::smallDot) {
		Collectable::AddToScore(10);
		dotsToCollect--;
	}
	if (collectableType == Collectable::ECollectable::largeDot) {
		Collectable::AddToScore(50);
		Ghost::SetAllDeadModes(true);
		dotsToCollect--;
		bigDotsToCollect--;
	}
	if (collectableType == Collectable::ECollectable::fruit) {
		Collectable::AddToScore(100);
		componentManager.GetComponent<AudioGenerator>()->PlayClip("Eat_Fruit");
	}

	if (dotsToCollect == 0)
		Wall::DoFlashingAnimation();
}

void Collectable::Reset() {
	timeToSpawnCounter = 0;
	hasSpawned = collectableType != ECollectable::fruit;
	SetVisibility(hasSpawned);
}

void Collectable::Update() {
	Entity::Update();
	if (collectableType == ECollectable::fruit) {
		if (!hasSpawned) {
			if (timeToSpawnCounter++ == timeToSpawn) {
				hasSpawned = true;
				SetVisibility(true);
				timeToSpawnCounter = 0;
			}
		}
		else {
			if (timeToSpawnCounter++ == timeToDespawn) {
				hasSpawned = false;
				SetVisibility(false);
				timeToSpawnCounter = 0;
			}
		}
	}
}