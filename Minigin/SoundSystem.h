#pragma once
#include <string>
#include "Command.h"

using sound_id = unsigned short;

class SoundSystem
{
public:
	virtual ~SoundSystem() = default;
	virtual void Play(const sound_id id, const float volume) = 0;
	virtual sound_id RegisterAudio(const std::string& filePath) = 0;
	virtual void Mute(bool isMuted) = 0;
	virtual bool GetIsMuted() = 0;

};

class MuteSoundSystemCommand : public Command
{
public:
	void Execute() override;

};

