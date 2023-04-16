#include "Sprite.h"
#include <iostream>
#include "Common.h"
#include "SceneManager.h"

SDL_Texture *Sprite::tex; //pointer to the SDL_Texture
SDL_Renderer *Sprite::renderer; //pointer to the renderer from main, link made in: SetupSprites()
int Sprite::outputSize = 64;
Coord Sprite::topLeftOffset = { 0, 0 };
bool Sprite::updateSize = false;

int Sprite::SetupSprites(SDL_Window *win, SDL_Renderer *renderer)
{
	SDL_Surface* surface = IMG_Load(SPRITE_SHEET_PATH);
	if (surface == nullptr) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(win);
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	tex = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (tex == nullptr) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	Sprite::renderer = renderer;
	return 0;
}

void Sprite::HandleResize(Coord size) {
	//This code makes the game stay in aspect ratio while filling as much screen space as it can
	if (size.x >= size.y) {
		outputSize = size.y / TILES_PER_ROW;
		topLeftOffset.x = (int)(0.5*(size.x - size.y)) + (size.y - outputSize*TILES_PER_ROW)/2;
		topLeftOffset.y = 0 + (size.y - outputSize*TILES_PER_ROW) / 2;
	} else {
		outputSize = size.x / TILES_PER_ROW;
		topLeftOffset.x = 0 + (size.x - outputSize*TILES_PER_ROW) / 2;
		topLeftOffset.y = (int)(0.5*(size.y - size.x)) + (size.x - outputSize*TILES_PER_ROW) / 2;
	}
	for (auto& obj : SceneManager::currentScene->sceneObjs) { 
		auto spriteComps = *obj->componentManager.GetComponents<Sprite>();
		for (unsigned int i = 0; i < spriteComps.size(); i++)
			spriteComps[i]->oldPos = glm::vec2{ -1,-1 };
	}
}

void Sprite::StaticCleanup() {
	if (tex != nullptr) 
		SDL_DestroyTexture(tex);
}

void Sprite::SetSpriteCoord(Coord c) {
	spriteCoord = c;
	srcImage.x = TILE_SIZE * c.x;
	srcImage.y = TILE_SIZE * c.y;
}

Coord Sprite::GetSpriteCoord() {
	return spriteCoord;
}

void Sprite::Render()
{
	if (!isVisible)
		return;
	if (oldPos != *pos) {
		destImage = {  
			topLeftOffset.x + (int)((pos->x + offsetFromPos.x) * outputSize), 
			topLeftOffset.y + (int)((pos->y + offsetFromPos.y) * outputSize), 
			outputSize, outputSize 
		};
	}
	//Draw the texture
	if (rotation == 0 && flip == SDL_FLIP_NONE)
		SDL_RenderCopy(renderer, tex, &srcImage, &destImage);
	else
		SDL_RenderCopyEx(renderer, tex, &srcImage, &destImage, rotation, NULL, flip);
	oldPos = *pos;
}
