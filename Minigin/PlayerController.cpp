#include "PlayerController.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Xinput.h"
#include <iostream>


class PlayerController::impl
{
public:

	PlayerController::impl(PlayerController* pPlayerController, bool isKeyboard, int id) :
		m_pPlayerController{ pPlayerController },
		m_Id{ id },
		m_IsKeyboard{ isKeyboard }
	{
	}

	bool Verify()
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(m_Id, &state) == ERROR_SUCCESS)
		{
			m_IsValid = true;
			return m_IsValid;
		}
		m_IsValid = false;
		return m_IsValid;
	}

	void PollState()
	{
		CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
		XInputGetState(m_Id, &m_currentState);

		WORD buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;
		m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
		m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);
	}

	void PollController(const SDL_Event& event = {})
	{
		if (m_pInputMapping)
		{
			if (m_IsKeyboard)
			{
				m_pInputMapping->PollInputMapping(event);
			}
			else
			{
				PollState();
				if (Verify())
				{
					m_pInputMapping->PollInputMapping(GetButtons(), GetButtonsPressedThisFrame(), GetButtonsReleasedThisFrame());
				}
				else
				{
					m_IsValid = false;
				}
			}
		}
	}

	void AddMapping(InputMapping* mapping)
	{
		m_pInputMapping.reset(mapping);
	}

	WORD GetButtonsPressedThisFrame() const
	{
		return m_buttonsPressedThisFrame;
	}

	WORD GetButtonsReleasedThisFrame() const
	{
		return m_buttonsReleasedThisFrame;
	}

	WORD GetButtons() const
	{
		return m_currentState.Gamepad.wButtons;
	}

private:
	PlayerController* m_pPlayerController;

	XINPUT_STATE m_currentState{};
	XINPUT_STATE m_previousState{};

	WORD m_buttonsPressedThisFrame{};
	WORD m_buttonsReleasedThisFrame{};

	bool m_IsKeyboard{};
	int m_Id{};
	bool m_IsValid{};
	std::unique_ptr<InputMapping> m_pInputMapping;


};

PlayerController::PlayerController(int id, bool isKeyboard):
	m_pImpl{ std::make_unique<impl>(this, isKeyboard, id) }
{

	std::cout << "PlayerController created with id " << id << std::endl;

	m_pImpl->Verify();
}

PlayerController::~PlayerController() = default;

void PlayerController::PollController(const SDL_Event& event)
{
	m_pImpl->PollController(event);
}

void PlayerController::AddMapping(InputMapping* mapping)
{
	m_pImpl->AddMapping(mapping);
}
