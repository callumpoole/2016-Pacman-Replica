#include "Animation.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "Common.h"
#include "Sprite.h"

Animation::Animation(Sprite* sprite) : sprite(sprite) {
	type = EComponentType::Animation; 
}

Animation::Animation(Sprite * sprite, const char * animFile) : sprite(sprite) {
	type = EComponentType::Animation;
	LoadFile(animFile);
}

Animation::Animation(const Animation & obj) {
	currentAnim = obj.currentAnim;
	ticksPerFrame = obj.ticksPerFrame;
	currentTicks = obj.currentTicks;
	queuedAnim = obj.queuedAnim;
	queuedLoop = obj.queuedLoop;
	queuedTicksPerFrame = obj.ticksPerFrame;
}

Animation::~Animation() {
	delete[] anims;
}

int Animation::LoadFile(const char * animFile) {
	std::string fad(ANIM_PATH);
	fad += animFile;
	const char* fileAndDir = fad.c_str();

	std::ifstream inFile(fileAndDir);
	int lines = (int)std::count(std::istreambuf_iterator<char>(inFile),
		std::istreambuf_iterator<char>(), '\n') + 1;
	anims = new Anim[lines];

	int lineNum = 0;
	std::ifstream in_stream;
	std::string line = "";
	try {
		in_stream.open(fileAndDir);
		while (!in_stream.eof())
		{
			in_stream >> line; //In the format of: x1,y1|x2,y2|x3,y3|...
			int numStages = 1;
			for (unsigned int i = 0; i < line.size(); i++)
				if (line[i] == '|') numStages++;
			anims[lineNum].stages = new Coord[numStages];

			int stageCount = 0;
			char* stage;

			//Split around the char '|'
			for (stage = strtok(const_cast<char*>(line.c_str()), "|"); stage; stage = strtok(NULL, "|")) {
				std::string sstage = std::string{ stage };
				int pos = sstage.find(',');
				anims[lineNum].stages[stageCount].x = std::stoi(sstage.substr(0, pos), nullptr, 10);
				anims[lineNum].stages[stageCount].y = std::stoi(sstage.substr(pos + 1, sstage.length() - pos + 1), nullptr, 10);
				stageCount++;
			}
			anims[lineNum].numberOfStages = stageCount;

			lineNum++;
		}
		numberOfAnims = lineNum;
		in_stream.close();
		return 0;
	}
	catch (...) {
		std::cout << "Error Loading animation file: " << fileAndDir << ".";
		system("pause");
		return -1;
	}
}

void Animation::Update() {
	if (currentTicks == 0) {
		//If animation has finished, then swap to the new one that was requested
		if (currentAnim != queuedAnim && anims[currentAnim].currentStage == anims[currentAnim].numberOfStages - 1) { 
			anims[currentAnim].currentStage = 0; //Reset for next time incase this anim gets replayed
			currentAnim = queuedAnim;
			loop = queuedLoop;
			ticksPerFrame = queuedTicksPerFrame;
		} else { 
			//If looping carry on. If not looping carry on until you hit the last frame then stick with a freeze frame until another animation is requested
			if (loop || (!loop && anims[currentAnim].currentStage < anims[currentAnim].numberOfStages-1))
				anims[currentAnim].currentStage = (anims[currentAnim].currentStage + 1) % anims[currentAnim].numberOfStages;
		}
		sprite->SetSpriteCoord(anims[currentAnim].stages[anims[currentAnim].currentStage]);
	}
	currentTicks = (currentTicks + 1) % ticksPerFrame;
}

void Animation::SetAnim(int animID, bool loop, bool forceAnim) {
	//If they don't specify a speed, then use the same speed as last time
	SetAnim(animID, loop, this->ticksPerFrame, forceAnim);
}

void Animation::SetAnim(int animID, bool loop, int ticksPerFrame, bool forceAnim) {
	if (currentAnim == -1 || animID == currentAnim || forceAnim) {
		currentAnim = animID;
		anims[currentAnim].currentStage = 0;
		queuedAnim = currentAnim;
		this->loop = loop;
		this->ticksPerFrame = ticksPerFrame;
		return;
	}
		
	queuedAnim = animID;
	queuedLoop = loop;
	queuedTicksPerFrame = ticksPerFrame;
}
