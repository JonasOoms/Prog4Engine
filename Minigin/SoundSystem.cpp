#include "SoundSystem.h"
#include "ServiceLocator.h"

void MuteSoundSystemCommand::Execute()
{
	SoundSystem* ss = ServiceLocator::GetSoundSystem();
	ss->Mute(!ss->GetIsMuted());
}
