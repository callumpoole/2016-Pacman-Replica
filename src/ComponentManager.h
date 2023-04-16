#pragma once
#include "Common.h"
#include "Component.h"
#include "InputController.h"
#include "SDL.h"

class Entity;

class ComponentManager
{
	Entity* entity;
	std::vector<std::unique_ptr<Component>> components;

public:
	ComponentManager(Entity* e) : entity(e) {}

	void Render() {
		for (auto& c : components)
			if (c->enabled)
				c->Render();
	}
	void Update() {
		for (auto& c : components)
			if (c->enabled)
				c->Update();
	}
	void GetKeyDown(SDL_Keycode key) {
		//Get Components of Type InputController and then call their GetKeyDown funcs
		if (auto ic = GetComponent<InputController>(EComponentType::InputController))
			ic->GetKeyDown(key);
	}

	template<typename T, typename... TArgs>
	T* AddComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = entity;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));
		return c;
	}

	template<typename T>
	T* GetComponent() {
		for (const auto& c : components) 
			if (T* ptr = dynamic_cast<T*>(c.get())) 
				return ptr;
		return nullptr;
	}
	
	//Made this to be more efficient than the one above
	template<typename T>
	T* GetComponent(EComponentType ct) {
		for (const auto& c : components) 
			if (c->type == ct)
				return dynamic_cast<T*>(c.get());
		return nullptr;
	}

	template<typename T>
	std::unique_ptr<std::vector<T*>> GetComponents() {
		std::unique_ptr<std::vector<T*>> uPtr(new std::vector<T*>());
		for (const auto& c : components)
			if (T* ptr = dynamic_cast<T*>(c.get()))
				uPtr->push_back(ptr);
		return uPtr;
	}
};
