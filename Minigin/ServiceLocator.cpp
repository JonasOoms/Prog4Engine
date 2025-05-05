#include "ServiceLocator.h"
#include "NullSoundSystem.h"
#include "NullPhysicsSystem.h"
std::unique_ptr<SoundSystem> ServiceLocator::m_pSoundSystem_Instance = std::make_unique<NullSoundSystem>();
std::unique_ptr<PhysicsSystem> ServiceLocator::m_pPhysicsSystem_Instance = std::make_unique<NullPhysicsSystem>();
