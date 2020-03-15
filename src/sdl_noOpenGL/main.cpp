#include <algorithm>
#include <chrono>
#include <SDL_mixer.h>
#include "Common.h"
#include "SceneManager.h"
#include "LevelLoader.h"
#include "Sprite.h"
#include "DispText.h"
#include "Entity.h"
#include "AudioGenerator.h"
#include "InputController.h"
#include "MenuManager.h"
#include "Menu.h"
#include "MainMenu.h"
#include "PauseMenu.h" 
#include "KeyBindingMenu.h"

std::string exeName;
SDL_Window *win; //pointer to the SDL_Window
SDL_Renderer *renderer; //pointer to the SDL_Renderer
const float TIME_GAP = 12.f;
Coord prevWindowSize;
Coord oldMousePos;

SceneManager scene;
MenuManager menuManager;

bool done = false;
bool fullScreen = false;
bool paused = false;

int loadingBarCurChunk = 0;
const int LOADING_BAR_MAX_CHUNKS = 11;

void InitVars() { 
	AudioGenerator::SetPsuedoVolume(1); 

	std::shared_ptr<DispText> ptr(new DispText(SDL_Rect{ 2,33,4,1 }, std::string("Score: ")));
	scene.textObjs.push_back(std::move(ptr));

	std::shared_ptr<DispText> ptr2(new DispText(SDL_Rect{ 6,33,4,1 }, std::string("0")));
	ptr2->autoWidth = 1;
	ptr2->tag = "ScoreCount";
	scene.textObjs.push_back(std::move(ptr2));

	std::shared_ptr<DispText> ptr3(new DispText(SDL_Rect{ 15,18,4,1 }, std::string("READY?")));
	ptr3->SetCol(SDL_Color{ 255, 255, 0, 255 });
	ptr3->tag = "Ready";
	ptr3->isVisible = false;
	scene.textObjs.push_back(std::move(ptr3));

	std::shared_ptr<DispText> ptr4(new DispText(SDL_Rect{ 14,18,6,1 }, std::string("GAME OVER")));
	ptr4->SetCol(SDL_Color{ 255, 0, 0, 255 });
	ptr4->tag = "GameOver";
	ptr4->isVisible = false;
	scene.textObjs.push_back(std::move(ptr4));
}


void HandleInput() {
	//Event-based input handling
	//The underlying OS is event-based, so **each** key-up or key-down (for example)
	//generates an event.
	//  - https://wiki.libsdl.org/SDL_PollEvent
	//In some scenarios we want to catch **ALL** the events, not just to present state
	//  - for instance, if taking keyboard input the user might key-down two keys during a frame
	//    - we want to catch based, and know the order
	//  - or the user might key-down and key-up the same within a frame, and we still want something to happen (e.g. jump)
	//  - the alternative is to Poll the current state with SDL_GetKeyboardState


	SDL_PumpEvents();
	Coord pos;
	SDL_GetMouseState(&pos.x, &pos.y);
	if (oldMousePos.x != pos.x || oldMousePos.y != pos.y) {
		menuManager.MouseMove(pos);

		oldMousePos = pos;
	}


	SDL_Event event; //somewhere to store an event

	//NOTE: there may be multiple events per frame
	while (SDL_PollEvent(&event)) //loop until SDL_PollEvent returns 0 (meaning no more events)
	{
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			menuManager.MouseDown(pos);
			break;
		case SDL_MOUSEBUTTONUP:
			menuManager.MouseUp(pos);
			break;
		case SDL_QUIT:
			done = true; //set donecreate remote branch flag if SDL wants to quit (i.e. if the OS has triggered a close event,
						 //  - such as window close, or SIGINT
			break;

			//keydown handling - we should to the opposite on key-up for direction controls (generally)
		case SDL_KEYDOWN:
			//Keydown can fire repeatable if key-repeat is on.
			//  - the repeat flag is set on the keyboard event, if this is a repeat event
			//  - in our case, we're going to ignore repeat events
			//  - https://wiki.libsdl.org/SDL_KeyboardEvent
			if (!event.key.repeat) {

				if (KeyBindingMenu::lineIDToWrite != -1) {
					InputController::SetKeyBinding(KeyBindingMenu::lineIDToWrite, event.key.keysym.sym);
					dynamic_cast<KeyBindingMenu*>(MenuManager::curManager->menus[MenuManager::IND_KeyMenu].get())->HidePressKeyMessage();

					KeyBindingMenu::lineIDToWrite = -1;
					return;
				}
				else if (event.key.keysym.sym == InputController::fullScreen) {
					fullScreen = !fullScreen;
					if (fullScreen)
						SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
					else {
						SDL_SetWindowFullscreen(win, 0);
						SDL_SetWindowPosition(win, 100, 100);
					}
				}

				menuManager.curManager->KeyDown(event.key.keysym.sym);

				if (menuManager.menus[MenuManager::IND_MainMenu]->GetVisibility()) //If the main menu is showing, don't accept any inputs except mouse
					return;

				scene.GetKeyDown(event.key.keysym.sym);

				switch (event.key.keysym.sym)
				{
					//hit escape to exit
				case SDLK_EQUALS: case SDLK_PLUS: 
					AudioGenerator::IncreaseVolumeBy(10);
					break;
				case SDLK_MINUS: case SDLK_UNDERSCORE:
					AudioGenerator::IncreaseVolumeBy(-10);
					break;
				case SDLK_ESCAPE:
					paused = !paused;
					scene.SetAllEntitiesFrozenMovement(paused);
					if (paused)
						menuManager.menus[MenuManager::IND_PauseMenu]->ShowMenu(); //Pause Menu
					else
						menuManager.menus[MenuManager::IND_PauseMenu]->HideMenu(); //Pause Menu
					break;
				}
			}
			break;
		}
	}
}

void HandleResize() {
	//Note that this kind of resizes keeps the game in the correct aspect ratio,
	//I could have easily made it stretch, but I thought that this would look better.
	int x, y;
	SDL_GetWindowSize(win, &x, &y);
	Coord curWindowSize = { x, y };
	if (curWindowSize.x != prevWindowSize.x || curWindowSize.y != prevWindowSize.y) {
		std::cout << "\rWindow Size change: [" << curWindowSize.x << "   " << curWindowSize.y << "]\t\t\t\t\n";
		Sprite::HandleResize(curWindowSize);
		DispText::HandleResize(curWindowSize);
	}
	
	prevWindowSize = curWindowSize;
}

void UpdateSimulation() {
	scene.Update();
	menuManager.Update();
	if (menuManager.shouldCloseGame)
		done = true;
	if (menuManager.setPauseState != -1) {
		paused = menuManager.setPauseState == 0 ? false : true;
		menuManager.setPauseState = -1;
	}
}

int SetupWindowAndRenderer() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	std::cout << "SDL initialised OK!\n";

	//create window
	win = SDL_CreateWindow("Pacman!", 700, 100, 1100, 800, SDL_WINDOW_RESIZABLE);

	//error handling
	if (win == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	std::cout << "SDL CreatedWindow OK!\n";
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	return 0;
}


void Render() {
	//First clear the renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//Render stuff
	scene.Render();

	menuManager.Render();

	//Update the screen
	SDL_RenderPresent(renderer);
}

void CleanExit(int returnValue) {
	LevelLoader::Unload();
	Menu::StaticMenuCleanup();
	AudioGenerator::StaticCleanup();
	DispText::StaticCleanup();
	Sprite::StaticCleanup();

	scene.Cleanup();
	menuManager.Cleanup();
	if (renderer != nullptr) 
		SDL_DestroyRenderer(renderer);
	if (win != nullptr) 
		SDL_DestroyWindow(win);

	SDL_Quit();
	exit(returnValue);
}

void IncrementLoadingBar() {
	loadingBarCurChunk++;
	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	SDL_Rect dest = { 0, h/2-7, (int)(w * (loadingBarCurChunk/(float)LOADING_BAR_MAX_CHUNKS)), 15 };
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderFillRect(renderer, &dest);
	SDL_RenderPresent(renderer);
}

// based on http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world/
int main( int argc, char* args[] ) {
	if (SetupWindowAndRenderer() != 0)
		return -1;

	//Setup Sprite Rendering, Text Rendering and Audio Production
	Sprite::SetupSprites(win, renderer);
			IncrementLoadingBar(); 
	DispText::SetupDispText(renderer);
			IncrementLoadingBar();
	AudioGenerator::Init();
			IncrementLoadingBar();
	Menu::StaticMenuSetup(renderer);
			IncrementLoadingBar();
	
	//Load the level and place the entities in the scene
	scene.Init();
			IncrementLoadingBar();
	LevelLoader::LoadFile("level1.lvl");
			IncrementLoadingBar();
	LevelLoader::PlaceAllTiles(scene.sceneObjs);
			IncrementLoadingBar();
	scene.SortSceneObjects();
			IncrementLoadingBar();
	menuManager.Init();
			IncrementLoadingBar();
	InitVars();
			IncrementLoadingBar();

	float currentTimeGap = 0.f;
	int lastFrameTime = 0;
	std::chrono::time_point<std::chrono::steady_clock> lastTimePoint = std::chrono::high_resolution_clock::now();
	std::chrono::duration<long long, std::nano> deltaTime;
	float fps;
	scene.SetAllEntitiesFrozenMovement(true);
	menuManager.menus[MenuManager::IND_MainMenu]->ShowMenu(); //Main Menu
			IncrementLoadingBar();

	while (!done) { 
		deltaTime = std::chrono::high_resolution_clock::now() - lastTimePoint;
		lastTimePoint = std::chrono::high_resolution_clock::now();

		HandleInput(); //Get the keyboard input
		HandleResize(); //Handle the window being resized

		currentTimeGap += lastFrameTime;
		for (; currentTimeGap >= TIME_GAP; currentTimeGap -= TIME_GAP)
			UpdateSimulation(); //Update the simulation as many times as it needs from the time that has past

		Render(); //Render the graphics

		lastFrameTime = (int)std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(deltaTime).count();
		fps = 1 / (lastFrameTime / 1000.f);
	}
	
	CleanExit(0);
	return 0;
}
