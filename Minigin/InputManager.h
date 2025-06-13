#pragma once
#include <map>
#include <vector>
#include "Singleton.h"
#include "PlayerController.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:

		InputManager();

		bool ProcessInput();

		PlayerController* GetPlayerController(int index);
		void ClearAllMappings();
		

	private:

		std::unique_ptr<PlayerController> m_pKeyboardPlayerController;
		std::map<int, std::unique_ptr<PlayerController>> m_pPlayerControllers;
	};

}
