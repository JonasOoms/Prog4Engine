#pragma once
#include <memory>
#include "SoundSystem.h"
#include "PhysicsSystem.h"
#include "NullSoundSystem.h"
#include "NullPhysicsSystem.h"

class ServiceLocator final
{
	static std::unique_ptr<SoundSystem> m_pSoundSystem_Instance;
	static std::unique_ptr<PhysicsSystem> m_pPhysicsSystem_Instance;
public:
	static SoundSystem* GetSoundSystem()
	{
		return m_pSoundSystem_Instance.get();
	}
	static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem)
	{
		if (pSoundSystem == nullptr)
		{
			m_pSoundSystem_Instance = std::make_unique<NullSoundSystem>();
			return;
		} 
		m_pSoundSystem_Instance = std::move(pSoundSystem);
	}

	static PhysicsSystem* GetPhysicsSystem()
	{
		return m_pPhysicsSystem_Instance.get();
	}

	static void RegisterPhysicsSystem(std::unique_ptr<PhysicsSystem>&& pPhysicsSystem)
	{
		if (pPhysicsSystem == nullptr)
		{
			m_pPhysicsSystem_Instance = std::make_unique<NullPhysicsSystem>();
			return;
		}
		m_pPhysicsSystem_Instance = std::move(pPhysicsSystem);
	}
};

