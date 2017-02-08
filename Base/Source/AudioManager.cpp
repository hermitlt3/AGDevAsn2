#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{

}

bool AudioManager::Init()
{
	Sound_Engine = createIrrKlangDevice();
	if (!Sound_Engine)
	{
		std::cout << "Sound Engine has failed to Initialise." << std::endl;
		return false;
	}
	BGM = Sound_Engine->play2D("Music/UpontheSky.mp3", false, false, true);
	if (!BGM)
	{
		std::cout << "No BGM" << std::endl;
		return false;
	}
	BGM->setVolume(0.2f);
	return true;
}

void AudioManager::Destroy()
{
	if (Sound_Engine)
		Sound_Engine->drop();
	if (BGM)
		BGM->drop();
}