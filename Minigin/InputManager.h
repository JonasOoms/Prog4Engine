#pragma once
#include "Singleton.h"
#include <vector>
#include "PlayerController.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:

		InputManager();

		bool ProcessInput();

		PlayerController* GetPlayerController(int index);
	private:

		std::unique_ptr<PlayerController> m_pKeyboardPlayerController;
		std::vector<std::unique_ptr<PlayerController>> m_pPlayerControllers;
	};

}
