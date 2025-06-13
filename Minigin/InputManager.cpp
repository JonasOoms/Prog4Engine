#include <SDL.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "InputManager.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "Xinput.h"

dae::InputManager::InputManager()
{
	m_pKeyboardPlayerController = std::make_unique<PlayerController>(-1, true);
	

}

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			m_pKeyboardPlayerController->PollController(e);
		}
		if (e.type == SDL_KEYUP)
		{
			m_pKeyboardPlayerController->PollController(e);
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		
		
		// process input for IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);

	}
		m_pKeyboardPlayerController->PollController(e);


		for (auto& controller : m_pPlayerControllers)
		{
			controller.second->PollController();
		}

	return true;
}

PlayerController* dae::InputManager::GetPlayerController(int index)
{
	if (index == -1)
	{
		return m_pKeyboardPlayerController.get();
	}
	if (!m_pPlayerControllers.contains(index))
	{
		m_pPlayerControllers.emplace(index, std::make_unique<PlayerController>(index, false));
	}
	return m_pPlayerControllers[index].get();
}

void dae::InputManager::ClearAllMappings()
{
	m_pKeyboardPlayerController->ClearMapping();
	for (auto& controller : m_pPlayerControllers)
	{
		controller.second->ClearMapping();
	}
}
