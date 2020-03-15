#include "DispText.h"
#include <iostream>
#include "Sprite.h"

SDL_Renderer *DispText::renderer;
Coord DispText::topLeftOffset;
Coord DispText::winSize;
int DispText::gameBoxSize;


int DispText::SetupDispText(SDL_Renderer *renderer) {
	DispText::renderer = renderer;
	if (TTF_Init() == -1) {
		std::cout << "TTF_Init Failed: " << TTF_GetError() << std::endl;
		return -1;
	}
	return 0;
}

void DispText::HandleResize(Coord size) {
	winSize = size;
	if (size.x >= size.y) {
		gameBoxSize = size.y;
		topLeftOffset.x = (int)(0.5*(size.x - size.y)) + (size.y - Sprite::outputSize*Sprite::TILES_PER_ROW) / 2;
		topLeftOffset.y = 0 + (size.y - Sprite::outputSize*Sprite::TILES_PER_ROW) / 2;
	} else {
		gameBoxSize = size.x;
		topLeftOffset.x = 0 + (size.x - Sprite::outputSize*Sprite::TILES_PER_ROW) / 2;
		topLeftOffset.y = (int)(0.5*(size.y - size.x)) + (size.x - Sprite::outputSize*Sprite::TILES_PER_ROW) / 2;
	}
}

DispText::DispText(SDL_Rect rect, std::string text, int size, SDL_Color col, std::string typeface) :
	message_rect(rect), text(text), size(size), col(col), typeface(typeface)
{
	GenerateTexture();
}

int DispText::GenerateTexture() {
	TTF_Font* sans = TTF_OpenFont(typeface.c_str(), size);
		sans = TTF_OpenFont(typeface.c_str(), size);
	if (sans == nullptr) {
		std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		return -1;
	}
	SDL_Surface *messageSurface = TTF_RenderText_Solid(sans, text.c_str(), col);
	messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
	return 0;
}

void DispText::SetFont(std::string text, int size, SDL_Color col) {
	SetFont(text, size, col, typeface);
}

void DispText::SetFont(std::string text, int size, SDL_Color col, std::string typeface) {
	this->text = text;
	this->typeface = typeface;
	this->size = size;
	this->col = col;
	GenerateTexture();
}

void DispText::Render() {
	if (!isVisible)
		return;
	if (autoWidth != -1)
		message_rect.w = text.length() * autoWidth;

	if (isDrawnInGameView) {
		float scalerX = gameBoxSize / (float)winSize.x;
		float scalerY = gameBoxSize / (float)winSize.y;
		float size = (float)(winSize.x > winSize.y ? winSize.y : winSize.x); //Min(x,y)
		SDL_Rect dest = { 
			topLeftOffset.x + message_rect.x * Sprite::outputSize,
			topLeftOffset.y + message_rect.y * Sprite::outputSize,
			message_rect.w * Sprite::outputSize, message_rect.h * Sprite::outputSize
		};
		if (isCentralised) {
			dest.x -= dest.w / 2;
			dest.y -= dest.h / 2;
		}
		SDL_RenderCopy(renderer, messageTexture, NULL, &dest);
	}
	else {
		if (isCentralised) {
			SDL_Rect dest = message_rect; //Make a copy
			dest.x -= dest.w / 2;
			dest.y -= dest.h / 2;
			SDL_RenderCopy(renderer, messageTexture, NULL, &dest);
		}
		else 
			SDL_RenderCopy(renderer, messageTexture, NULL, &message_rect);
	}
} 

void DispText::CleanUp() {
	if (messageTexture != nullptr) 
		SDL_DestroyTexture(messageTexture);
}

void DispText::StaticCleanup() {
	TTF_Quit();
}