#include "SceneManager.h"
#include "Common.h"
#include "HealthBar.h"
#include "LevelLoader.h"
#include "DispText.h"

SceneManager* SceneManager::currentScene;

void SceneManager::Update() {
	for (auto& ob : sceneObjs) 
		ob->Update();
}

void SceneManager::GetKeyDown(SDL_Keycode key) {
	for (auto& ob : sceneObjs)
		ob->componentManager.GetKeyDown(key);
}

void SceneManager::RestartLevel(int level){
	this->level = level;
	LevelLoader::PlaceAllTiles(this->sceneObjs, true);
}

void SceneManager::Cleanup() {
	for (auto& ob : sceneObjs)
		ob->CleanUp();
	for (auto& te : textObjs) 
		te->CleanUp();
	sceneObjs.clear();
	textObjs.clear();
}

void SceneManager::SortSceneObjects() {
	std::vector<std::shared_ptr<Entity>> tempAllObjs = sceneObjs;
	sceneObjs = std::vector<std::shared_ptr<Entity>>();
	for (int i = 0; i <= Entity::GetMaxRenderingLayer(); i++) {
		for (auto& obj : tempAllObjs) {
			if (obj->GetRenderingLayer() == i)
				sceneObjs.push_back(obj);
		}
	}
}

int SceneManager::RemoveEntityWithID(int id) {
	for (unsigned int i = 0; i < sceneObjs.size(); i++) {
		if (sceneObjs[i]->id == id) {
			sceneObjs.erase(sceneObjs.begin() + i);
			return 0;
		}
	}
	return -1;
}

void SceneManager::SetAllEntitiesFrozen(bool state) {
	for (auto& obj : sceneObjs)
		obj->SetFreeze(state);
}

void SceneManager::SetAllEntitiesFrozenMovement(bool state) {
	for (auto& obj : sceneObjs)
		obj->SetFreezeMovement(state);
}


void SceneManager::Render() {
	for (auto& ob : sceneObjs) 
		ob->Render();
	for (auto& te : textObjs) 
		te->Render();
}


SceneManager::SceneManager() {
	currentScene = this;
}


SceneManager::~SceneManager() {
	Cleanup();
}


std::vector<std::shared_ptr<Entity>> SceneManager::GetEntitiesOfType(Entity::EEntityType type) {
	std::vector<std::shared_ptr<Entity>> list;
	for (auto& ob : sceneObjs)
		if (ob->entityType == type) 
			list.push_back(ob);
	return list;
}

std::vector<std::shared_ptr<Entity>> SceneManager::GetEntitiesWithTag(std::string tag) {
	std::vector<std::shared_ptr<Entity>> list;
	for (auto& ob : sceneObjs)
		if (ob->tag == tag) 
			list.push_back(ob);
	return list;
}

std::shared_ptr<DispText> SceneManager::GetDispTextWithTag(std::string tag){
	for (auto& ob : textObjs)
		if (ob->tag == tag)
			return ob;
	return nullptr;
}

int SceneManager::Init() {
	std::shared_ptr<HealthBar> ptr(new HealthBar(glm::vec2(31,33)));
	sceneObjs.push_back(std::move(ptr));

	return 0;
}