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
#include "ActivationBoxComponent.h"
#include <iostream>

GameModeSelectorLoadingComponent::GameModeSelectorLoadingComponent(TRONGame* game):
	m_Game{game}
{
}

void GameModeSelectorLoadingComponent::EndPlay()
{
	m_Selector->Destroy();
}

void GameModeSelectorLoadingComponent::BeginPlay()
{

	dae::Scene& scene = dae::SceneManager::GetInstance().GetCurrentScene();

	auto go = std::make_unique<dae::GameObject>();
	auto singlePlayerBox = go->AddComponent<ActivationBoxComponent>([&]()
		{
			m_Game->ChangeGameMode(GameMode::Singleplayer);
			dae::SceneManager::GetInstance().SelectScene("Level");
			m_Game->SetIsInLevel(true);
		});
	go->AddComponent<PhysicsComponent>(glm::vec2{ 130,100 });
	go->SetPosition(dae::Minigin::windowWidth / 2 - 130/2 - 20, dae::Minigin::windowHeight / 2 - 130);
	scene.Add(std::move(go));

	auto tank = std::make_unique<dae::GameObject>();
	m_Selector = tank.get();
	
	tank->AddComponent<PhysicsComponent>(glm::vec2{ 20.f, 20.f });
	auto playerControllerComponentKeyboard = tank->AddComponent<PlayerControllerComponent>(100.f);
	auto selectorRenderer = tank->AddComponent<RenderComponentEx>(20.f, 20.f);
	selectorRenderer->SetTexture("Textures/T_Selector.png");
	tank->SetPosition(dae::Minigin::windowWidth/2 - 20.f/2, dae::Minigin::windowHeight/2 - 20.f/2);


	auto inputMappingKeyboard = new InputMapping();
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_W, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,-1 })));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_D, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_S, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_A, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_SPACE, TriggerType::Pressed, std::make_unique<ActivateCommand>(tank.get(), std::vector{ singlePlayerBox }));

	dae::InputManager::GetInstance().GetPlayerController(-1)->AddMapping(inputMappingKeyboard);
	

	auto inputMappingController = new InputMapping();
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_UP, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 0,-1 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_RIGHT, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_DOWN, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_LEFT, TriggerType::Pressed, std::make_unique<PlayerControllerMoveCommand>(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_A, TriggerType::Pressed, std::make_unique<ActivateCommand>(tank.get(), std::vector{ singlePlayerBox }));
	dae::InputManager::GetInstance().GetPlayerController(0)->AddMapping(inputMappingController);
	
	scene.Add(std::move(tank));

	ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("GameIntro"), 50.f);

}
