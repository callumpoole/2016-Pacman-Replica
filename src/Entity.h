#pragma once
#include <glm\glm.hpp>
#include "Common.h"
#include "ComponentManager.h"

class Entity
{
public:
	enum class EEntityType {
		None = 0,
		player = 1,
		ghost = 2,
		collectable = 3,
		wall = 4,
		other = 5
	};
private:
	int renderingLayer = 0;
	static int maxRenderingLayer;
protected:
	glm::vec2 spawnPos = { 0,0 };
	static int idCount;
	bool isAlive = true;
	bool isMovementFrozen = false;
	bool isFrozen = false;
public:
	ComponentManager componentManager = ComponentManager(this);
	int id = -1;
	EEntityType entityType = EEntityType::None;

	Entity() {}
	Entity(glm::vec2 pos, EEntityType et = EEntityType::None) : pos(pos), entityType(et) {
		id = idCount++;
		spawnPos = pos;
	}

	std::string tag = "";
	glm::vec2 pos; //Position
	glm::vec2 vel; //Velocity
 
	inline void SetAlive(bool alive) { isAlive = alive; }
	inline bool IsAlive() const { return isAlive; }
	inline void DeleteMe() { isAlive = false; }
	inline void SetFreeze(bool freeze) { isFrozen = freeze; }
	inline bool GetFreeze() const { return isFrozen; }
	inline void SetFreezeMovement(bool freezeMovement) { isMovementFrozen = freezeMovement; }
	inline bool GetFreezeMovement() const { return isMovementFrozen; }

	void SetRenderingLayer(int layer);
	inline int GetRenderingLayer() const { return renderingLayer; }
	static inline int GetMaxRenderingLayer() { return maxRenderingLayer; }

	void SetVisibility(bool vis);

	virtual void Reset() {}
	virtual void Render();
	virtual void Update();
	virtual void CleanUp() {}
};
