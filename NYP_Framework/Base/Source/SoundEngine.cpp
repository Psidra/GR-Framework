#include "SoundEngine.h"
#include <iostream>

using namespace std;

// Constructor
CSoundEngine::CSoundEngine()
	: SoundEngine(NULL)
{
}

// Destructor
CSoundEngine::~CSoundEngine()
{
	// Clear out the sound map
	soundMap.clear();

	// Delete the sound engine
	if (SoundEngine)
	{
		delete SoundEngine;
		SoundEngine = NULL;
	}
}

// Init this class and it will create the Sound Engine
bool CSoundEngine::Init(void)
{
	// Create the sound engine
	SoundEngine = createIrrKlangDevice();
	if (!SoundEngine)
		return false;	// error starting up the sound engine

	return true;
}

// Get the handler to the sound engine
ISoundEngine* CSoundEngine::GetSoundEngine(void)
{
	if (SoundEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::GetSoundEngine() - The Sound Engine has not been initialised yet." << endl;
			return NULL;
		}
	}

	return SoundEngine;
}

// Add a sound to this library
void CSoundEngine::AddSound(const std::string& _soundIndex, const std::string& _soundSource)
{
	if (SoundEngine == NULL)
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
std::string CSoundEngine::GetSound(const std::string& _soundIndex)
{
	if (SoundEngine == NULL)
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
bool CSoundEngine::RemoveSound(const std::string& _soundIndex)
{
	if (SoundEngine == NULL)
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
void CSoundEngine::PlayASound(const std::string& _soundIndex)
{
	std::string aSound = GetSound(_soundIndex);
	if (!SoundEngine->isCurrentlyPlaying(aSound.c_str()))
	{
		// Play a sound
		SoundEngine->play2D(aSound.c_str(), false, false);
	}
}