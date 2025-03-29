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
	m_pPlayerControllers.reserve(XUSER_MAX_COUNT);

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_pPlayerControllers.push_back(std::make_unique<PlayerController>(i, false));
	}

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

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_pPlayerControllers[i]->PollController();
	}


	return true;
}

PlayerController* dae::InputManager::GetPlayerController(int index)
{
	if (index == -1)
	{
		return m_pKeyboardPlayerController.get();
	}
	return m_pPlayerControllers[index].get();
}
