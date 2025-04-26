#pragma once
#include "SoundSystem.h"
#include <memory>
#include <string>

class SDL_SoundSystem: public SoundSystem
{

public:
	SDL_SoundSystem();
	~SDL_SoundSystem();

	// Inherited via SoundSystem
	void Play(const sound_id id, const float volume) override;
	sound_id RegisterAudio(const std::string& filePath) override;
private:
	class impl; 
	std::unique_ptr<impl> m_pImpl;
};

