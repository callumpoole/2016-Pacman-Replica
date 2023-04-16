#include "Player.h"
#include "LevelLoader.h"
#include "SceneManager.h"
#include "Collectable.h"
#include "AudioGenerator.h"
#include "Ghost.h"
#include "Common.h"
#include "HealthBar.h"
#include "MenuManager.h"
#include "HighScoresMenu.h"
#include "Component.h"
#include "Sprite.h"
#include "Animation.h"
#include "CollisionBox.h"

std::vector<Player*> Player::players;
bool Player::respawnStage1 = true;
const char* Player::PACMAN_ANIM_FILE = "pacman.anim";
const char* Player::MRS_PACMAN_ANIM_FILE = "mrs_pacman.anim";
const float Player::SPEED = 0.05f;
void PlayerOnCollide(Entity* self, CollisionBox* cb);

Player::Player() : Entity() {
}

Player::Player(glm::vec2 pos, bool player1) : Entity(pos, EEntityType::player), player1(player1) {
	spawnPos = pos;
	SetRenderingLayer(3);

	Sprite* s = componentManager.AddComponent<Sprite>(&this->pos);
	s->SetSpriteCoord(Coord{ 0, 1 + player1 ? 0 : 1 });

	Animation* a = componentManager.AddComponent<Animation>(s);
	a->LoadFile(player1 ? PACMAN_ANIM_FILE : MRS_PACMAN_ANIM_FILE);
	a->SetAnim(0, true, 8);

	componentManager.AddComponent<InputController>(player1 ? 0 : 1);

	if (player1) {
		currentDir = chosenDir = EDirection::Left;
	} else {
		currentDir = chosenDir = EDirection::Right;
		s->flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	}
	MoveDirection(currentDir);

	componentManager.AddComponent<CollisionBox>(dynamic_cast<Entity*>(this), &PlayerOnCollide);

	componentManager.AddComponent<AudioGenerator>();

	players.push_back(this);
}


void Player::Kill() {
	SceneManager::currentScene->SetAllEntitiesFrozenMovement(true);
	componentManager.GetComponent<Animation>(EComponentType::Animation)->SetAnim(1, false);
	isDying = true;
	HealthBar::currentHealthBar->RemoveLive();
	componentManager.GetComponent<AudioGenerator>()->PlayClip("Die");
}

void Player::Respawn() {
	auto ghosts = SceneManager::currentScene->GetEntitiesOfType(EEntityType::ghost);

	if (respawnStage1) { //Respawn Stage 1: Hide the ghosts, display "Ready?"
		respawnStage1 = false;
		Collectable::curFruit->Reset();
		if (HealthBar::currentHealthBar->GetLives() > 0) {
			//Display "Ready"
			SceneManager::currentScene->GetDispTextWithTag("Ready")->isVisible = true;
		} else {
			//Display "Game Over"
			SceneManager::currentScene->GetDispTextWithTag("GameOver")->isVisible = true;
		}
		
		for (auto& p : players)
			p->SetVisibility(false);
		for (auto& g : ghosts)
			g->SetVisibility(false);
		
	} else { //Respawn Stage 2: Show Ghosts, hide "Ready", reset positions and rotations for players and ghosts
		if (HealthBar::currentHealthBar->GetLives() <= 0) { //Game Over
			//Stuff to reset
			Collectable::curFruit->SetAlive(false); //Stop the cherry from appearing on the text

			MenuManager::curManager->menus[MenuManager::IND_EnterNameMenu]->ShowMenu();
			return;
		}
		SceneManager::currentScene->GetDispTextWithTag("Ready")->isVisible = false;
		respawnStage1 = true; //Reset for next time
		SceneManager::currentScene->SetAllEntitiesFrozenMovement(false);
		isDying = false;

		//Reset both players
		for (auto& p : players) {
			p->SetVisibility(true);
			p->componentManager.GetComponent<Animation>(EComponentType::Animation)->SetAnim(0, true);
			p->pos = p->spawnPos;
			p->chosenDir = p->player1 ? EDirection::Left : EDirection::Right;
			p->MoveDirection(chosenDir);
		}

		//Reset the ghosts
		for (auto& g : ghosts) {
			g->SetVisibility(true);
			g->Reset();
		}
	}
}

void Player::SelectDirection(EDirection direction)
{
	chosenDir = direction;
}

void Player::CheckIfMove()
{
	bool moveInNewDir = false;

	//If they want to go a different direction, see if they can:
	if (chosenDir != currentDir) {
		switch (chosenDir)
		{
		case EDirection::None:
			break;
		case EDirection::Up:
			if (!LevelLoader::IsWall(LevelLoader::grid[gridCoord.y - 1][gridCoord.x], true)) 
				moveInNewDir = true;
			break;
		case EDirection::Down:
			if (!LevelLoader::IsWall(LevelLoader::grid[gridCoord.y + 1][gridCoord.x], true)) 
				moveInNewDir = true;
			break;
		case EDirection::Left:
			if (!LevelLoader::IsWall(LevelLoader::grid[gridCoord.y][gridCoord.x - 1], true)) 
				moveInNewDir = true;
			break;
		case EDirection::Right:
			if (!LevelLoader::IsWall(LevelLoader::grid[gridCoord.y][gridCoord.x + 1], true)) 
				moveInNewDir = true;
			break;
		default:
			break;
		}
	}

	if (moveInNewDir)
		if (abs(pos.x - (float)gridCoord.x) < 0.2f && abs(pos.y - (float)gridCoord.y) < 0.2f)
			MoveDirection(chosenDir);

	bool getReadyToStop = false;
	//If the player is moving, then check if they're walking into a wall
	switch (currentDir)
	{
	case EDirection::None:
		break;
	case EDirection::Up:
		if (LevelLoader::IsWall(LevelLoader::grid[gridCoord.y - 1][gridCoord.x], true)) 
			getReadyToStop = true;
		break;
	case EDirection::Down:
		if (LevelLoader::IsWall(LevelLoader::grid[gridCoord.y + 1][gridCoord.x], true)) 
			getReadyToStop = true;
		break;
	case EDirection::Left:
		if (LevelLoader::IsWall(LevelLoader::grid[gridCoord.y][gridCoord.x - 1], true)) 
			getReadyToStop = true;
		break;
	case EDirection::Right:
		if (LevelLoader::IsWall(LevelLoader::grid[gridCoord.y][gridCoord.x + 1], true)) 
			getReadyToStop = true;
		break;
	default:
		break;
	}

	if(getReadyToStop)
		if (abs(pos.x - (float)gridCoord.x) < 0.2f && abs(pos.y - (float)gridCoord.y) < 0.2f)
			MoveDirection(EDirection::None);
}

void Player::MoveDirection(EDirection direction) {
	currentDir = direction;
	auto s = componentManager.GetComponent<Sprite>(EComponentType::Sprite);
	switch (direction) {
	case EDirection::None:
		vel = glm::vec2(0, 0);
		//Keep the current rotation
		break;
	case EDirection::Up:
		vel = glm::vec2(0, -SPEED);
		s->rotation = 90;
		s->flip = SDL_RendererFlip::SDL_FLIP_NONE;
		pos.x = round(pos.x);
		break;
	case EDirection::Down:
		vel = glm::vec2(0, SPEED);
		s->rotation = -90;
		s->flip = SDL_RendererFlip::SDL_FLIP_NONE;
		pos.x = round(pos.x);
		break;
	case EDirection::Left:
		vel = glm::vec2(-SPEED, 0);
		s->rotation = 0;
		s->flip = SDL_RendererFlip::SDL_FLIP_NONE;
		pos.y = round(pos.y);
		break;
	case EDirection::Right:
		vel = glm::vec2(SPEED, 0);
		s->rotation = 0;
		s->flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		pos.y = round(pos.y);
		break;
	}
}

void PlayerOnCollide(Entity* self, CollisionBox* collision)
{
	Player* player = dynamic_cast<Player*>(self);
	if (collision->entity->entityType == Entity::EEntityType::collectable) {
		collision->entity->SetVisibility(false);
		
		if (Collectable* c = dynamic_cast<Collectable*>(collision->entity))
			if (c->HasSpawned()) {
				c->Collect();
				player->ticksSinceLastDot = 0;
				AudioGenerator::Play("Waka");
			}
		SceneManager::currentScene->GetDispTextWithTag("ScoreCount")->SetText(std::to_string(Collectable::GetScore()));
		collision->entity->SetAlive(false);
	} else if (collision->entity->entityType == Entity::EEntityType::ghost) {
		Ghost* ghost = dynamic_cast<Ghost*>(collision->entity);
		if (player->isDying)
			return;
		if (ghost->GetDeadMode()) { //Kill Ghost
			if (!ghost->GetIsRunningBackToSpawn()) {
				ghost->Kill();
				self->componentManager.GetComponent<AudioGenerator>()->PlayClip("Eat_Ghost");
			}
		} else //Kill Player
			player->Kill();
	}
}

void Player::Update() {
	Entity::Update();

	gridCoord = { (int)round(pos.x), (int)round(pos.y) };
	CheckIfMove();
	if (isDying) {
		if (respawnTimer++ == RESPAWN_TICKS) {
			respawnTimer = 0;
			Respawn();
		}
	}
}