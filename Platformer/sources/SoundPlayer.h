#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <map>
#include <SDL.h>
#include <SDL_mixer.h>

class SoundPlayer
{
public:
	static SoundPlayer* getInstance();
	void addSound(std::string pSoundName, std::string pSoundPath);
	void playSound(std::string pSoundName);
private:
	SoundPlayer();

	static SoundPlayer* mInstance;
	std::map<std::string, Mix_Chunk*> mLibraray;
};

#endif