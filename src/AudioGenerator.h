

#pragma once
#include "Common.h"
#include "Component.h"
#include <map>

#ifdef _WIN32 // compiling on windows
	#include <SDL_mixer.h>
#else // NOT compiling on windows
	#include <SDL2/SDL_mixer.h>
#endif

//Learning resource: http://lazyfoo.net/tutorials/SDL/21_sound_effects_and_music/index.php

class AudioGenerator : public Component
{
private:
	//The sound effects that will be used
	static std::map<std::string, Mix_Music*> musicClips;
	static std::map<std::string, Mix_Chunk*> audioClips;
	static std::string currentSongName;
	static char volume;
public:
	AudioGenerator();
	~AudioGenerator();

	static int Init();
	static inline bool IsPlaying() { return Mix_PausedMusic() != 1; }

	//Music
	static int LoadMusic();
	static int StaticCleanup();
	static void Play(std::string clip, int loop = -1);
	static void TogglePauseResume();
	static void Stop();


	static int SetVolume(char volume);
	static int IncreaseVolumeBy(int i);
	static int SetPsuedoVolume(int i); //Where 0 <= i <= 10
	static int GetPsuedoVolume();
	static int IncreasePsuedoVolumeBy(int i); 

	//Sound FX
	static int LoadSoundEffects();
	static int UnloadSoundEffects();
	void PlayClip(std::string clip, int channel = -1);
};
