#include "ServiceLocator.h"
#include "NullSoundSystem.h"

std::unique_ptr<SoundSystem> ServiceLocator::m_pSoundSystem_Instance = std::make_unique<NullSoundSystem>();
