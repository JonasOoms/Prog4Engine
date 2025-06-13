#define WIN32_LEAN_AND_MEAN

#include "Minigin.h"
#include <Windows.h>
#include <Xinput.h>
#include "GameModeSelectorLoadingComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "RenderComponentEx.h"
#include "InputManager.h"
#include "PlayerControllerComponent.h"
#include "PlayerControllerMoveCommand.h"
#include "PhysicsComponent.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"
#include "GameRegistries.h"
#include <iostream>

GameModeSelectorLoadingComponent::GameModeSelectorLoadingComponent(TRONGame* game, dae::GameObject* selector, const std::vector<ActivationBoxComponent*>& activationBoxes) :
	m_Game{ game },
	m_Selector{ selector }
{
	m_ActivationBoxes = activationBoxes;
}

void GameModeSelectorLoadingComponent::Update(float)
{
	if (m_GotoLevel)
	{
		dae::SceneManager::GetInstance().SelectScene("Level");
		//m_Game->SetIsInLevel(true);
	}
}

void GameModeSelectorLoadingComponent::EndPlay()
{
}

void GameModeSelectorLoadingComponent::BeginPlay()
{



	dae::InputManager::GetInstance().ClearAllMappings();
	//dae::Scene& scene = dae::SceneManager::GetInstance().GetCurrentScene();


	

	auto playerControllerComponentKeyboard = m_Selector->GetComponent<PlayerControllerComponent>();

	auto inputMappingKeyboard = std::make_unique<InputMapping>();
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_W, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,-1 })));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_D, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_S, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_A, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_SPACE, TriggerType::Pressed, std::make_unique<ActivateCommand>(m_Selector, m_ActivationBoxes));
	inputMappingKeyboard->AddInputBinding(SDLK_F2, TriggerType::Released, std::make_unique<MuteSoundSystemCommand>());

	dae::InputManager::GetInstance().GetPlayerController(-1)->AddMapping(std::move(inputMappingKeyboard));
	

	auto inputMappingController = std::make_unique<InputMapping>();
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_UP, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 0,-1 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_RIGHT, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_DOWN, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_LEFT, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_A, TriggerType::Pressed, std::make_unique<ActivateCommand>(m_Selector, m_ActivationBoxes));
	dae::InputManager::GetInstance().GetPlayerController(0)->AddMapping(std::move(inputMappingController));
	
	ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("GameIntro"), 20.f);

}
