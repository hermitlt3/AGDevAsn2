#pragma once
#include "SingletonTemplate.h"
#include <string>
#include "irrKlang.h"
using namespace irrklang;

class AudioManager : public Singleton<AudioManager>
{
	friend class Singleton<AudioManager>;

public:
	AudioManager();
	~AudioManager();

	bool Init();
	void Destroy();

	ISoundEngine* Sound_Engine;
	ISound* BGM;
};