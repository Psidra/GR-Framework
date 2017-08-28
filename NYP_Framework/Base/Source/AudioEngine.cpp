#include "DetectMemoryLeak.h"
#include "AudioEngine.h"
#include <iostream>

using namespace std;

// Constructor
AudioEngine::AudioEngine()
	: iAudioEngine(NULL)
{
}

// Destructor
AudioEngine::~AudioEngine()
{
	// Clear out the sound map
	soundMap.clear();

	// Delete the sound engine
	if (iAudioEngine)
	{
		delete iAudioEngine;
		iAudioEngine = NULL;
	}
}

// Init this class and it will create the Sound Engine
bool AudioEngine::Init(void)
{
	// Create the sound engine
	iAudioEngine = createIrrKlangDevice();
	if (!iAudioEngine)
		return false;	// error starting up the sound engine

	return true;
}

// Get the handler to the sound engine
ISoundEngine* AudioEngine::GetAudioEngine(void)
{
	if (iAudioEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::GetSoundEngine() - The Sound Engine has not been initialised yet." << endl;
			return NULL;
		}
	}

	return iAudioEngine;
}

// Add a sound to this library
void AudioEngine::AddSound(const std::string& _soundIndex, const std::string& _soundSource)
{
	if (iAudioEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::AddSound() - The Sound Engine has not been initialised yet." << endl;
			return;
		}
	}

	// Clean up first if there is an existing goodie with the same name
	RemoveSound(_soundIndex);
	
	// Add the mesh now
	soundMap[_soundIndex] = _soundSource;
}

// Get a sound from this map 
std::string AudioEngine::GetSound(const std::string& _soundIndex)
{
	if (iAudioEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::GetSound() - The Sound Engine has not been initialised yet." << endl;
			return std::string();
		}
	}

	if (soundMap.count(_soundIndex) != 0)
	{
		return soundMap[_soundIndex];
	}

	return std::string();
}

// Remove a sound from this map
bool AudioEngine::RemoveSound(const std::string& _soundIndex)
{
	if (iAudioEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::RemoveSound() - The Sound Engine has not been initialised yet." << endl;
			return NULL;
		}
	}

	std::string aSoundName = GetSound(_soundIndex);
	if (!aSoundName.empty())
	{
		soundMap.erase(_soundIndex);
		return true;
	}
	
	return false;
}

// Play a sound from this map
void AudioEngine::PlayASound(const std::string& _soundIndex, bool _loop)
{
	std::string aSound = GetSound(_soundIndex);
	if (!iAudioEngine->isCurrentlyPlaying(aSound.c_str()))
	{
		// Play a sound
		iAudioEngine->play2D(aSound.c_str(), _loop, false);
	}
}

void AudioEngine::StopAllSounds()
{
	iAudioEngine->stopAllSounds();
}

bool AudioEngine::CheckSoundOn(const std::string & _soundIndex)
{
	std::string aSound = GetSound(_soundIndex);
	return iAudioEngine->isCurrentlyPlaying(aSound.c_str());
}

int AudioEngine::getVolume()
{
	return (iAudioEngine->getSoundVolume() * 100);
}

void AudioEngine::setVolume(int _volume)
{
	if (_volume > 100)
		_volume = 100;

	if (_volume < 0)
		_volume = 0;

	iAudioEngine->setSoundVolume((float)_volume * 0.01f);
}

void AudioEngine::editVolume(int _volume)
{
	int currentVolume = getVolume();
	currentVolume += _volume;

	if (currentVolume > 100)
		currentVolume = 100;

	if (currentVolume < 0)
		currentVolume = 0;

	iAudioEngine->setSoundVolume((float)currentVolume * 0.01f);
}
