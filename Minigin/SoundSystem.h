#pragma once
#include <string>
using sound_id = unsigned short;

class SoundSystem
{
public:
	virtual ~SoundSystem() = default;
	virtual void Play(const sound_id id, const float volume) = 0;
	virtual sound_id RegisterAudio(const std::string& filePath) = 0;
	

};

