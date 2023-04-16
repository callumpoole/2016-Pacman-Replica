#include "LevelLoader.h"
#include "Common.h"
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>

#include "Player.h"
#include "Ghost.h"
#include "Wall.h"
#include "Collectable.h"
#include "Teleporter.h"

const char* LevelLoader::LEVELS_PATH = "Resources\\Levels\\";
LevelLoader::EEntityType** LevelLoader::grid;
int LevelLoader::gridSize = 0;

typedef wchar_t ucs4_t;

void LevelLoader::LoadFile(const char* filename) {
	//Concatenate the strings
	std::string sURI(LEVELS_PATH);
	sURI += filename;
	const char* URI = sURI.c_str();

	//Get how many lines there are
	std::ifstream inFile(URI);
	gridSize = (int)std::count(std::istreambuf_iterator<char>(inFile),
		std::istreambuf_iterator<char>(), '\n') + 1;

	//Allocate the memory
	grid = new EEntityType*[gridSize];
	for (int i = 0; i < gridSize; i++)
		grid[i] = new EEntityType[gridSize];

	//Open the file and read into the grid
	std::locale old_locale;
	std::locale utf8_locale(old_locale, new std::codecvt_utf8<wchar_t>);
	std::wifstream input_file(URI);
	input_file.imbue(utf8_locale);
	ucs4_t item = 0;
	EEntityType curType = EEntityType::empty;
	int charCounter = 0;
	int rowCounter = 0;
	while (input_file >> item) {
		switch (item) {
		case '^':	curType = EEntityType::empty; break;
		case 9474:  curType = EEntityType::vertical; break;
		case 9472:  curType = EEntityType::horizontal; break;
		case 9484:  curType = EEntityType::topLeft; break;
		case 9488:  curType = EEntityType::topRight; break;
		case 9492:  curType = EEntityType::bottomLeft; break;
		case 9496:  curType = EEntityType::bottomRight; break;
		case '~':	curType = EEntityType::ghostBarrier; break;
		case 'P':	curType = EEntityType::player1Spawn; break;
		case 'p':	curType = EEntityType::player2Spawn; break;
		case 'g':	curType = EEntityType::ghostSpawn; break;
		case '.':	curType = EEntityType::dot; break;
		case 'o':	curType = EEntityType::bigDot; break;
		case 'f':	curType = EEntityType::fruit; break;
		case '*':	curType = EEntityType::teleporter; break;
		}
		grid[rowCounter][charCounter] = curType;

		if (++charCounter == gridSize) {
			charCounter = 0;
			rowCounter++;
		}
	}
}

void LevelLoader::PlaceAllTiles(std::vector<std::shared_ptr<Entity>>& sceneObjs, bool onlyDots) {
	int ghostCounter = 0;

	for (int y = 0; y < gridSize; y++) {
		for (int x = 0; x < gridSize; x++) {

			if (onlyDots && !(grid[y][x] == LevelLoader::EEntityType::dot || 
							  grid[y][x] == LevelLoader::EEntityType::bigDot || 
							  grid[y][x] == LevelLoader::EEntityType::fruit))
			{
				continue;
			}
			
			switch (grid[y][x])
			{
				case LevelLoader::EEntityType::empty:
					break;
				case LevelLoader::EEntityType::vertical:
				case LevelLoader::EEntityType::horizontal:
				case LevelLoader::EEntityType::topLeft:
				case LevelLoader::EEntityType::topRight:
				case LevelLoader::EEntityType::bottomLeft:
				case LevelLoader::EEntityType::bottomRight:
				case LevelLoader::EEntityType::ghostBarrier:
				{
					std::shared_ptr<Wall> ptr(new Wall(glm::vec2(x, y), (EWallType)(int)grid[y][x]));
					sceneObjs.push_back(std::move(ptr));
				} break;
				case LevelLoader::EEntityType::player1Spawn:
				case LevelLoader::EEntityType::player2Spawn:
				{
					std::shared_ptr<Player> ptr(new Player(glm::vec2(x, y), grid[y][x] == LevelLoader::EEntityType::player1Spawn));
					sceneObjs.push_back(std::move(ptr));
				} break;
				case LevelLoader::EEntityType::ghostSpawn:
				{
					std::shared_ptr<Ghost> ptr(new Ghost(glm::vec2(x, y), Ghost::EGhostType(++ghostCounter)));
					sceneObjs.push_back(std::move(ptr));
				} break;
				case LevelLoader::EEntityType::dot:
				case LevelLoader::EEntityType::bigDot:
				case LevelLoader::EEntityType::fruit:
				{
					std::shared_ptr<Collectable> ptr(new Collectable(glm::vec2(x, y), (Collectable::ECollectable)(int(grid[y][x])-int(EEntityType::dot)+1)));
					sceneObjs.push_back(std::move(ptr));
				} break;
				case LevelLoader::EEntityType::teleporter:
				{
					std::shared_ptr<Teleporter> ptr(new Teleporter(glm::vec2(x,y)));
					sceneObjs.push_back(std::move(ptr));
				} break;
			}
		}
	}
}

bool LevelLoader::IsWall(int x, int y, bool incBarrier) {
	if (y >= 0 && y < gridSize && x >= 0 && x < gridSize)
		return IsWall(grid[y][x], incBarrier);
	else
		return false;
}

int LevelLoader::IsJunction(int x, int y, EDirection dirs[4]) {
	int neighbours = 0;
	if (IsWall(x, y, false))
		return 0;

	if (!IsWall(x, y - 1)) dirs[neighbours++] = EDirection::Up;
	if (!IsWall(x, y + 1)) dirs[neighbours++] = EDirection::Down;
	if (!IsWall(x - 1, y)) dirs[neighbours++] = EDirection::Left;
	if (!IsWall(x + 1, y)) dirs[neighbours++] = EDirection::Right;
	return neighbours;
}