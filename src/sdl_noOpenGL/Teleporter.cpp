#include "Teleporter.h"
#include "CollisionBox.h"
#include "Sprite.h"

Teleporter::Teleporter(glm::vec2 pos, glm::vec2 disp) : Entity(pos, Entity::EEntityType::other), displacementness(disp) {
	//If left teleporter
	if (pos.x < Sprite::TILES_PER_ROW / 2)
		displacementness = glm::vec2(28, 0); //Teleport Right
	//If right teleporter
	else
		displacementness = glm::vec2(-28, 0); //Teleport Left

	componentManager.AddComponent<CollisionBox>(this, &TeleporterOnCollide);
}


void TeleporterOnCollide(Entity* entity, CollisionBox* cb) {
	if (cb->entity->entityType == Entity::EEntityType::player || cb->entity->entityType == Entity::EEntityType::ghost) {
		if (Teleporter* t = dynamic_cast<Teleporter*>(entity))
			cb->entity->pos += t->displacementness;
	}
}