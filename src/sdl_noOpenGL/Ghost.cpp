#include "Ghost.h"
#include "LevelLoader.h"
#include "CollisionBox.h"
#include "Collectable.h"
#include "Component.h"
#include "Sprite.h"
#include "Animation.h"

int Ghost::killStreak = 0;
std::vector<Ghost*> Ghost::ghosts;
int Ghost::NumGhosts = 0;
const float Ghost::SPEED = 0.05f;
const float Ghost::SLOW_SPEED = 0.025f;
const float Ghost::FAST_SPEED = 0.075f;
const float Ghost::PERCENT_FLASHING = 0.33f;
const int Ghost::DEAD_MODE_DURATION = 1000;
void GhostOnCollide(Entity* self, CollisionBox* cb);

Ghost::Ghost()
{
}

Ghost::Ghost(glm::vec2 pos, EGhostType type) : Entity(pos, EEntityType::ghost), type(type) {
	SetRenderingLayer(2);
	NumGhosts++;
	Sprite* s_ghost = componentManager.AddComponent<Sprite>(&this->pos);
	Sprite* s_eyes = componentManager.AddComponent<Sprite>(&this->pos);
	s_ghost->SetSpriteCoord(1, 2 + (int)type);

	SetDirection((EDirection)(NumGhosts%2+1), true); //Alternate Up then Down as the ghosts spawn in

	Animation* a = componentManager.AddComponent<Animation>(s_ghost);
	a->LoadFile("ghosts.anim");
	a->SetAnim(int(type)-1, true, 40);

	CollisionBox* cb = componentManager.AddComponent<CollisionBox>(dynamic_cast<Entity*>(this), &GhostOnCollide);

	srand((unsigned int)time(0));
	targetBounces = (NumGhosts - 1) * 10 + rand() % 15;

	ghosts.push_back(this);
}

void Ghost::SetDirection(EDirection Direction, bool setVel = false) {
	this->Direction = Direction;
	(*componentManager.GetComponents<Sprite>())[1]->SetSpriteCoord(3, 3 + (int)((int)Direction-1));
	if (setVel)
		switch (Direction)
		{
		case EDirection::None:  vel = glm::vec2(0, 0);      break;
		case EDirection::Up:    vel = glm::vec2(0, -speed); break;
		case EDirection::Down:  vel = glm::vec2(0, speed);  break;
		case EDirection::Left:  vel = glm::vec2(-speed, 0); break;
		case EDirection::Right: vel = glm::vec2(speed, 0);  break;
		default: break;
		}
}

void Ghost::SetAllDeadModes(bool mode){
	for (Ghost* g : ghosts)
		if (!g->isRunningBackToSpawn)
			g->SetDeadMode(mode);
}

void Ghost::SetDeadMode(bool mode) {
	if (isRunningBackToSpawn)
		return;
	isDead = mode;
	isDeadAndFlashing = false;
	(*componentManager.GetComponents<Sprite>())[0]->enabled = true;
	if (mode) {
		//TODO: Change Direction and Update Speed In SetDirection with it
		Ghost::speed = SLOW_SPEED;
		SetDirection(Direction, true);
		//Set to blue sprites
		(*componentManager.GetComponents<Sprite>())[0]->SetSpriteCoord(1, 7);
		componentManager.GetComponent<Animation>()->SetAnim(4, true, 40, true);
		(*componentManager.GetComponents<Sprite>())[1]->enabled = false;
	} else {
		deadModeTimer = 0;
		Ghost::speed = SPEED;
		SetDirection(Direction, true);
		killStreak = 0;
		//Set sprites back
		(*componentManager.GetComponents<Sprite>())[0]->SetSpriteCoord(1, 2 + int(type));
		componentManager.GetComponent<Animation>()->SetAnim(int(type) - 1, true, 40, true);
		(*componentManager.GetComponents<Sprite>())[1]->enabled = true;
	}
}




void Ghost::Kill(){
	killStreak++;
	Collectable::AddToScore(killStreak * 200);
	isRunningBackToSpawn = true;
	(*componentManager.GetComponents<Sprite>())[0]->enabled = false;
	(*componentManager.GetComponents<Sprite>())[1]->enabled = true;
	speed = FAST_SPEED;
	SetDirection(Direction, true);
}

void Ghost::Reset() {
	//Reset ghosts to their initial state
	isDead = isDeadAndFlashing = false;
	deadModeTimer = 0;
	inSpawn = true;
	isExitingSpawn = isRunningBackToSpawn = false;
	currentBouncesInSpawn = 0;
	speed = SPEED;
	pos = spawnPos;
	SetDirection((int)type % 2 == 0 ? EDirection::Up : EDirection::Down, true);
	(*componentManager.GetComponents<Sprite>())[0]->enabled = true;
	(*componentManager.GetComponents<Sprite>())[1]->enabled = true;
	componentManager.GetComponent<Animation>()->SetAnim(int(type) - 1, true, 40);
}

void Ghost::HideAllGhosts() {
	for (auto& g : ghosts)
		g->SetVisibility(false);
}

void Ghost::MoveInSpawn()
{                            
	if (isExitingSpawn) {
		SetDirection(EDirection::Up, true);
		//If a barrier is below them... then they're in the main game
		if (LevelLoader::grid[gridCoord.y + 1][gridCoord.x] == LevelLoader::EEntityType::ghostBarrier && pos.y <= (float)gridCoord.y) { //If there's a ghost barrier below and they've gone past half way
			isExitingSpawn = false;
			inSpawn = false;
			if (gridCoord.x == 16)
				SetDirection(EDirection::Left, true);
			else if (gridCoord.x == 17)
				SetDirection(EDirection::Right, true);
		}
	} else if (Direction == EDirection::Up || Direction == EDirection::Down) {
		if (CanMoveInDirection(true))
			vel = glm::vec2(0, speed * (Direction == EDirection::Up ? -1 : 1));
		else {
			currentBouncesInSpawn++;
			SetDirection(Common::OppositeDirection(Direction));
		}
	} else if (Direction == EDirection::Left || Direction == EDirection::Right)
		SetDirection(EDirection::Up);
	
	if (currentBouncesInSpawn == targetBounces)
		isExitingSpawn = true;
}

void Ghost::CheckIfMove() {
	glm::vec2 gridPos = { (float)gridCoord.x, (float)gridCoord.y };
	EDirection dirs[4];
	int juncs = LevelLoader::IsJunction(gridCoord.x, gridCoord.y, dirs);
	if ((juncs > 2 || (juncs == 2 && dirs[0] != Common::OppositeDirection(dirs[1]))) && oldGridPos != gridPos) { //If at a junction and not the same junction again
		if (!GonePastHalfWayOfTile())
			return;
		int juncsAvailable = juncs;
		for (int i = 0; i < juncs; i++)
			if (Common::OppositeDirection(Direction) == dirs[i]) {
				dirs[i] = EDirection::None;
				juncsAvailable--;
			}
		srand((unsigned int)time(NULL));
		int selectedDirIndex = rand() % juncsAvailable;
		if (dirs[selectedDirIndex] != EDirection::None)
			SetDirection(dirs[selectedDirIndex], true);
		else
			SetDirection(dirs[selectedDirIndex + 1], true);
		pos = gridPos;
		oldGridPos = gridPos;
	}
}

bool Ghost::CanMoveInDirection(bool incBarrriers)
{
	switch (Direction)
	{
	case EDirection::Up:    return !LevelLoader::IsWall(gridCoord.x, gridCoord.y - 1, incBarrriers);
	case EDirection::Down:  return !LevelLoader::IsWall(gridCoord.x, gridCoord.y + 1, incBarrriers);
	case EDirection::Left:  return !LevelLoader::IsWall(gridCoord.x - 1, gridCoord.y, incBarrriers);
	case EDirection::Right: return !LevelLoader::IsWall(gridCoord.x + 1, gridCoord.y, incBarrriers);
	}
	return false;
}

void Ghost::Update()
{
	Entity::Update();
	gridCoord = { (int)round(pos.x), (int)round(pos.y) };

	//Dead Mode Stuff
	if (isDead) {
		deadModeTimer++;
		if (!isDeadAndFlashing)
			if (deadModeTimer >= (int)(DEAD_MODE_DURATION - DEAD_MODE_DURATION * PERCENT_FLASHING)) {
				isDeadAndFlashing = true;
				//Set flashing on:
				componentManager.GetComponent<Animation>()->SetAnim(5, true, 15);
			}
		if (deadModeTimer >= DEAD_MODE_DURATION)
			SetDeadMode(false);
	}

	if (inSpawn)
		MoveInSpawn();
	else
		CheckIfMove();
}

bool Ghost::GonePastHalfWayOfTile() {
	switch (Direction)
	{
	case EDirection::Up:    return pos.y <= (float)gridCoord.y;
	case EDirection::Down:  return pos.y >= (float)gridCoord.y;
	case EDirection::Left:  return pos.x <= (float)gridCoord.x;
	case EDirection::Right: return pos.x >= (float)gridCoord.x;
	}
	return true;
}

void GhostOnCollide(Entity* self, CollisionBox* cb) {

}