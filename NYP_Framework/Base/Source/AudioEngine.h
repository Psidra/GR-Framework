#pragma once

#include "SingletonTemplate.h"
#include <irrKlang.h>
using namespace irrklang;
#include <map>

class AudioEngine : public Singleton<AudioEngine>
{
protected:
	// Destructor
	ISoundEngine* iAudioEngine;

	// The library map of all the sounds created
	std::map<std::string, std::string> soundMap;

public:
	// Constructor
	AudioEngine();
	virtual ~AudioEngine();

	// Init this class and it will create the Sound Engine
	bool Init(void);

	// Get the handler to the sound engine
	ISoundEngine* GetAudioEngine(void);

	// Add a sound to this library
	void AddSound(const std::string& _soundIndex, const std::string& _soundSource);
	// Get a sound from this map 
	std::string GetSound(const std::string& _soundIndex);
	// Remove a sound from this map
	bool RemoveSound(const std::string& _soundIndex);
	// Play a sound from this map
	void PlayASound(const std::string& _soundIndex, bool _loop = false);
	// Stop all sound
	void StopAllSounds();
	// Check if a sound is playing
	bool CheckSoundOn(const std::string& _soundIndex);

	// Get Volume (0-100)
	int getVolume();
	// Set Volume (0-100)
	void setVolume(int _volume);
	// Edit Volume (0-100)
	void editVolume(int _volume);
};