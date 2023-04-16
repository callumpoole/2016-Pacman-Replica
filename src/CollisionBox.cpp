#include "CollisionBox.h"
#include "Entity.h"

std::vector<CollisionBox*> CollisionBox::collBoxes;

CollisionBox::CollisionBox(Entity* e, void(colFunc)(Entity* entity, CollisionBox* collision)) {
	type = EComponentType::CollisionBox;
	entity = e;
	collisionFunc = colFunc;
	collBoxes.push_back(this);
}

void CollisionBox::Update() {
	if (slowUpdate++ % 12 != 0)
		return;

	for (unsigned int i = 0; i < collBoxes.size(); i++) {
		if (entity->id == collBoxes[i]->entity->id || !entity->IsAlive() || !collBoxes[i]->entity->IsAlive())
			continue;

		//Circle Collider, works well enough as the game is a grid with walls around everything
		if (glm::length(collBoxes[i]->entity->pos - entity->pos) < 0.5 && collBoxes[i]->entity->IsAlive())
			collisionFunc(entity, collBoxes[i]);
	}
}