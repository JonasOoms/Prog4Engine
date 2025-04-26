#include "InputMapping.h"
#include <iostream>


InputMapping::InputMapping()
{
}

InputMapping::~InputMapping()
{
}

void InputMapping::PollInputMapping(unsigned short buttons, unsigned short buttonsPressedThisFrame, unsigned short buttonsReleasedThisFrame)
{
	for (const auto& binding : m_InputBindings)
	{
		switch (binding.triggerType)
		{
		case TriggerType::Pressed:
			if (buttons & binding.button)
			{
				binding.command->Execute();
			}
			break;
		case TriggerType::Down:
			if (buttonsPressedThisFrame & binding.button)
			{
				binding.command->Execute();
			}
			break;
		case TriggerType::Released:
			if (buttonsReleasedThisFrame & binding.button)
			{
				binding.command->Execute();
			}
			break;
		}
	}
}

void InputMapping::PollInputMapping(const SDL_Event& event)
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	for (const auto& binding : m_InputBindings)
	{
		switch (binding.triggerType)
		{
		case TriggerType::Pressed:
			if (keyboardState[binding.button])
			{
				binding.command->Execute();
			}
			break;
		case TriggerType::Down:
			if (event.type == SDL_KEYDOWN)
			{
				if ((unsigned int)event.key.keysym.sym == binding.button)
				{
					binding.command->Execute();
				}
			}
			break;
		case TriggerType::Released:
			if (event.type == SDL_KEYUP)
			{
				if ((unsigned int)event.key.keysym.sym == binding.button)
				{
					binding.command->Execute();
				}
			}
			break;
		}
	}
}

