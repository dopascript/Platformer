#include "SoundPlayer.h"

SoundPlayer* SoundPlayer::mInstance = nullptr;

SoundPlayer* SoundPlayer::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new SoundPlayer();
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
	}
	return mInstance;
}

void SoundPlayer::addSound(std::string pSoundName, std::string pSoundPath)
{
	if (mLibraray.find(pSoundName) == mLibraray.end())
	{
		Mix_Chunk *lMix_Chunk;
		lMix_Chunk = Mix_LoadWAV(pSoundPath.c_str());
		mLibraray[pSoundName] = lMix_Chunk;
	}
}

void SoundPlayer::playSound(std::string pSoundName)
{
	Mix_PlayChannel(-1, mLibraray[pSoundName], 0);
}


SoundPlayer::SoundPlayer()
{

}