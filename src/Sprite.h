#pragma once
#include "Common.h"
#ifdef _WIN32 // compiling on windows
	#include <SDL_image.h>
#else // NOT compiling on windows
	#include <SDL2/SDL_image.h>
#endif
#include "Component.h"


class Sprite : public Component
{
private:
	static SDL_Texture *tex; //pointer to the SDL_Texture
	static SDL_Renderer *renderer; //pointer to the renderer from main, link made in: SetupSprites()

	const int TILE_SIZE = 64;

	SDL_Rect destImage{};
	Coord spriteCoord;

	glm::vec2 *pos; //This has been added to allow for floating point positions, which makes controlling speed easier

	SDL_Rect srcImage = { 0, 0, TILE_SIZE, TILE_SIZE };
public:
	bool isVisible = true;

	static Coord topLeftOffset;
	static const int TILES_PER_ROW = 34; //Game tiles that fill a row of the board
	static int outputSize; // = availablePixels / TILES_PER_ROW      <- Changes when resized
	static bool updateSize;
	glm::vec2 oldPos = { -1,-1 };
	glm::vec2 offsetFromPos; //Probably never going to use, but nice to have anyway...

	Sprite() { type = EComponentType::Sprite; }
	Sprite(glm::vec2* pos) : pos(pos) { type = EComponentType::Sprite; }

	static int SetupSprites(SDL_Window *win, SDL_Renderer *renderer);
	static void HandleResize(Coord size);
	static void StaticCleanup();

	inline void SetSpriteCoord(int x, int y) { SetSpriteCoord(Coord{ x, y }); }
	void SetSpriteCoord(Coord c);
	Coord GetSpriteCoord();
	void Render() override;
	
	double rotation = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

