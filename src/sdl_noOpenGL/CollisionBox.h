#pragma once
#include "Component.h"
#include "Common.h"

class Entity;
class CollisionBox : public Component
{
private:
	static std::vector<CollisionBox*> collBoxes;
	int slowUpdate = 0;
public:
	Entity* entity;

	void (*collisionFunc)(Entity* entity, CollisionBox* collision);

	CollisionBox(Entity* e, void(colFunc)(Entity* entity, CollisionBox* collision));

	void Update();
};

