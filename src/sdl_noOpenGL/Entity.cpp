#include "Entity.h"
#include "Component.h"
#include "Sprite.h"
#include "Animation.h"

int Entity::idCount = 0;
int Entity::maxRenderingLayer = 0;

void Entity::SetRenderingLayer(int layer) {
	if (layer > maxRenderingLayer)
		maxRenderingLayer = layer;
	renderingLayer = layer;
}

void Entity::SetVisibility(bool vis) {
	auto spriteComp = componentManager.GetComponents<Sprite>();
	for (unsigned int i = 0; i < (*spriteComp).size(); i++)
		(*spriteComp)[i]->isVisible = vis;
}

void Entity::Render() {
	if (isAlive)
		componentManager.Render();
}
void Entity::Update() {
	if (!isAlive && !isFrozen)
		return;
	if (!isMovementFrozen)
		pos += vel;
	componentManager.Update();
}