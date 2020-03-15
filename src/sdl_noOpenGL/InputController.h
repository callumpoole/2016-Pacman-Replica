#pragma once
#include "Component.h"
#include "SDL.h"
class InputController : public Component
{
public:
	static SDL_Keycode fullScreen;

	void GetKeyDown(SDL_Keycode key);
	SDL_Keycode up = SDLK_w;
	SDL_Keycode down = SDLK_s;
	SDL_Keycode left = SDLK_a;
	SDL_Keycode right = SDLK_d;
	
	InputController(int playerID);
	bool LoadKeyBindings(int playerID);

	static bool SetKeyBinding(int indexInFile, SDL_Keycode code);
};

