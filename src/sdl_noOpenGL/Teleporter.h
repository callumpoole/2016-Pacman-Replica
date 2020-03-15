#pragma once
#include "Entity.h"

class CollisionBox;

void TeleporterOnCollide(Entity* entity, CollisionBox* cb);
class Teleporter : public Entity
{
public:
	glm::vec2 displacementness;

	Teleporter(glm::vec2 pos, glm::vec2 disp = { 0,0 });
};

