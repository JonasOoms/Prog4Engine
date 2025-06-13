#pragma once
#include "SoundSystem.h"

class NullSoundSystem : public SoundSystem
{
public:
	void Play(const sound_id, const float) override {};
	virtual sound_id RegisterAudio(const std::string&) override { return 0; }

	// Inherited via SoundSystem
	void Mute(bool ) override {};
	bool GetIsMuted() override {return false;}
};

