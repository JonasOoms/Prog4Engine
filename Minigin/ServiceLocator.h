#pragma once
#include <memory>
#include "SoundSystem.h"
class ServiceLocator final
{
	static std::unique_ptr<SoundSystem> m_pSoundSystem_Instance;
public:
	static SoundSystem& GetSoundSystem()
	{
		return *m_pSoundSystem_Instance;
	}
	static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem)
	{
		m_pSoundSystem_Instance = std::move(pSoundSystem);
	}
};

