#pragma once
#include "Common.h"
#include "Entity.h"

class DispText;

class SceneManager
{
private:
	int level = 1;
public:
	static SceneManager* currentScene;

	std::vector<std::shared_ptr<Entity>> sceneObjs;
	std::vector<std::shared_ptr<DispText>> textObjs;
	
	std::vector<std::shared_ptr<Entity>> GetEntitiesOfType(Entity::EEntityType type);
	std::vector<std::shared_ptr<Entity>> GetEntitiesWithTag(std::string tag);

	std::shared_ptr<DispText> SceneManager::GetDispTextWithTag(std::string tag);
	
	void SortSceneObjects();
	int RemoveEntityWithID(int id);
	void SetAllEntitiesFrozen(bool state);
	void SetAllEntitiesFrozenMovement(bool state);

	int Init(); 
		
	void Render();
	void Update();
	void GetKeyDown(SDL_Keycode key);

	inline int GetLevel() { return level; }
	void RestartLevel(int level);

	void Cleanup();

	SceneManager();
	~SceneManager();
};
