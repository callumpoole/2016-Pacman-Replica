#pragma once
#include "Common.h"
#include "SDL.h"

enum class EComponentType {
	NullComponent,
	Sprite,
	Animation,
	InputController,
	AudioGenerator,
	CollisionBox
};

class Entity;
class Component
{
public:
	Entity* entity = nullptr;
	EComponentType type = EComponentType::NullComponent;
	bool enabled = true;

	Component() {}

	virtual inline void Update() {}
	virtual inline void Render() {}
	virtual inline void GetKeyDown(SDL_Keycode key) {}
};
