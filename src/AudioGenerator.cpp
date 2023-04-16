#include "AudioGenerator.h"
#include "MenuManager.h"
#include <math.h>
#include <algorithm>

#define AUDIO_PATH "Resources\\Audio\\"

std::map<std::string, Mix_Music*> AudioGenerator::musicClips;
std::map<std::string, Mix_Chunk*> AudioGenerator::audioClips;
std::string AudioGenerator::currentSongName = "";
char AudioGenerator::volume = MIX_MAX_VOLUME - 1;

AudioGenerator::AudioGenerator()
{
	type = EComponentType::AudioGenerator;
}

AudioGenerator::~AudioGenerator()
{
	UnloadSoundEffects();
}

int AudioGenerator::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return -1;
	}
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return -2;
	}
	LoadMusic();
	LoadSoundEffects();

	return 0;
}

int AudioGenerator::LoadMusic()
{
	int ret = 0; //return
	//Load music
	musicClips["Intermission"] = Mix_LoadMUS(AUDIO_PATH "Pacman_Intermission.wav");
	musicClips["Opening_Song"] = Mix_LoadMUS(AUDIO_PATH "Pacman_Opening_Song.wav");
	musicClips["Siren"] = Mix_LoadMUS(AUDIO_PATH "Pacman_Siren.wav");
	musicClips["Waka"] = Mix_LoadMUS(AUDIO_PATH "Pacman_Waka_Waka.wav");
	for (std::map<std::string, Mix_Music*>::iterator it = musicClips.begin(); it != musicClips.end(); it++)
		if (it->second == NULL) {
			std::cout << "Error: Loading Music Key: [" << it->first << "] = NULL";
			ret++;
		} 
	return ret;
}

int AudioGenerator::StaticCleanup()
{
	//Free the music
	for (std::map<std::string, Mix_Music*>::iterator it = musicClips.begin(); it != musicClips.end(); it++) {
		Mix_FreeMusic(it->second);
		it->second = NULL;
	}
	Mix_Quit();
	return 0;
}

void AudioGenerator::Play(std::string clip, int loop)
{
	if (clip == currentSongName)
		return;
	//Play the music for the first time
	if (Mix_PlayMusic(musicClips[clip], -1) == -1) {
		std::cout << "Mix_PlayMusic: %s\n" << Mix_GetError();
	}
	currentSongName = clip;
}

void AudioGenerator::TogglePauseResume()
{
	//If the music is paused
	if (Mix_PausedMusic() == 1)
	{
		//Resume the music
		Mix_ResumeMusic();
	}
	//If the music is playing
	else
	{
		//Pause the music
		Mix_PauseMusic();
	}
}

void AudioGenerator::Stop()
{
	Mix_HaltMusic();
}

int AudioGenerator::SetVolume(char volume) {
	AudioGenerator::volume = std::max(0, std::min((int)volume, MIX_MAX_VOLUME));
	for (int i = 0; i < 5; i++)
		Mix_Volume(i, volume);
	MenuManager::curManager->UpdateVolumeValue();
	return Mix_VolumeMusic(volume);
}
int AudioGenerator::IncreaseVolumeBy(int i) {
	AudioGenerator::volume += i;
	return SetVolume(volume);
}

//Ranges from 0 to 10 instead of 0 to 255, makes it more user friendly
int AudioGenerator::SetPsuedoVolume(int i) {
	return SetVolume(char(int((128.f / 10.f)*i)));
}
int AudioGenerator::GetPsuedoVolume() {
	return int(roundf((AudioGenerator::volume / 120.f)*10));
}
int AudioGenerator::IncreasePsuedoVolumeBy(int i) {
	int psuedoVolume = GetPsuedoVolume();
	if (i > 0 && psuedoVolume == 10)
		return -1;
	else
		return SetPsuedoVolume(GetPsuedoVolume() + i);
}

int AudioGenerator::LoadSoundEffects()
{
	int retVal = 0;
	//Resources
	audioClips["Die"] = Mix_LoadWAV(AUDIO_PATH "Pacman_Dies.wav");
	audioClips["Eat_Fruit"] = Mix_LoadWAV(AUDIO_PATH "Pacman_Eating_Cherry.wav");
	audioClips["Eat_Ghost"] = Mix_LoadWAV(AUDIO_PATH "Pacman_Eating_Ghost.wav");
	audioClips["Extra_Life"] = Mix_LoadWAV(AUDIO_PATH "Pacman_Extra_Live.wav");

	for (std::map<std::string, Mix_Chunk*>::iterator it = audioClips.begin(); it != audioClips.end(); it++) {
		if (it->second == NULL) {
			std::cout << "Error Loading SoundFX:" << it->first << ".\n";
			retVal++;
		}
	}

	return retVal;
}

int AudioGenerator::UnloadSoundEffects()
{
	//Free the sound effects
	for (std::map<std::string, Mix_Chunk*>::iterator it = audioClips.begin(); it != audioClips.end(); it++) {
		Mix_FreeChunk(it->second);
		it->second = NULL;
	}
	return 0;
}

void AudioGenerator::PlayClip(std::string clip, int channel) {
	Mix_PlayChannel(channel, audioClips[clip], 0);
}
