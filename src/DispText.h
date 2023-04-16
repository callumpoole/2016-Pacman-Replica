#pragma once
#include "Common.h"
#include "Widget.h"
#ifdef _WIN32 // compiling on windows
	#include <SDL_ttf.h>
#else // NOT compiling on windows
	#include <SDL2/SDL_ttf.h>
#endif

class DispText : public Widget
{
private:
	std::string text = "No Text";
	std::string typeface;
	int size; 
	SDL_Color col;

	SDL_Texture *messageTexture; //pointer to the SDL_Texture for message
	static SDL_Renderer *renderer; //pointer to the renderer from main

	static Coord topLeftOffset;
	static Coord winSize;
	static int gameBoxSize; 
public:
	bool isVisible = true;
	int autoWidth = -1; //-1 = off, n is pixels (width) per character
	std::string tag = "";
	static int SetupDispText(SDL_Renderer *renderer);
	static void HandleResize(Coord size);
	bool isDrawnInGameView = true;
	bool isCentralised = false;
	SDL_Rect message_rect; //SDL_rect for the message

	DispText(SDL_Rect rect, std::string text, int size = 100, SDL_Color col = { 255, 255, 255 }, std::string typeface = DEFAULT_TYPEFACE_PATH);

	int GenerateTexture();

	std::string GetText() const { return text; }
	std::string GetTypeface() const { return typeface; }
	int GetSize() const { return size; }
	SDL_Color GetCol() const { return col; }

	inline void SetText(std::string text) { this->text = text; GenerateTexture(); }
	inline void SetTypeface(std::string typeface) { this->typeface = typeface; GenerateTexture(); }
	inline void SetSize(int size) { this->size = size; GenerateTexture(); }
	inline void SetCol(SDL_Color col) { this->col = col; GenerateTexture(); }
	void SetFont(std::string text, int size, SDL_Color col);
	void SetFont(std::string text, int size, SDL_Color col, std::string typeface);

	void Render() override;

	void CleanUp();
	static void StaticCleanup();
};

