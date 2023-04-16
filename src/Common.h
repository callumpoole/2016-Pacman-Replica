#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <time.h>

#include <glm\glm.hpp>

#ifdef _WIN32 // compiling on windows
	#include <SDL.h>
#else // NOT compiling on windows
	#include <SDL2/SDL.h>
#endif

struct Coord { int x, y; };
enum class EDirection {
	None = 0,
	Up = 1,
	Down = 2,
	Left = 3,
	Right = 4
};
class Common {
public:
	static EDirection OppositeDirection(EDirection Direction) {
		switch (Direction)
		{
		case EDirection::Up:    return EDirection::Down;
		case EDirection::Down:  return EDirection::Up;
		case EDirection::Left:  return EDirection::Right;
		case EDirection::Right: return EDirection::Left;
		default:				return Direction;
		}
	}
};





