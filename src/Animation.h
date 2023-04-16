#pragma once
#include "Component.h"
#include <glm\glm.hpp>

class Sprite;

struct Anim {
	Coord* stages = nullptr;
	int numberOfStages = 0;
	int currentStage = 0;
};
class Animation : public Component
{
private:
	static const char* ANIM_PATH;
	Sprite* sprite;
	Anim* anims;
	int numberOfAnims;

	int currentAnim = -1;
	int ticksPerFrame = 40;
	int currentTicks = 1;
	bool loop;

	//This is set in queue so it allows the animation to finish before starting the next one
	int queuedAnim;
	bool queuedLoop;
	int queuedTicksPerFrame;
public:
	Animation() {}
	Animation(Sprite* sprite);
	Animation(Sprite* sprite, const char* animFile);
	Animation(const Animation& obj);
	~Animation();
	int LoadFile(const char * animFile);

	void Update() override;
	void SetAnim(int animID, bool loop, bool forceAnim = false);
	void SetAnim(int animID, bool loop, int ticksPerFrame, bool forceAnim = false); //If loop is off, then it will stay on the last frame
};