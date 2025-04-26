#pragma once
#include "SoundSystem.h"

class NullSoundSystem : public SoundSystem
{
	void Play(const sound_id, const float) override {};
	virtual sound_id RegisterAudio(const std::string&) override { return 0; }
};

