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


void GameModeSelectorLoadingComponent::EndPlay()
{
	m_Selector->Destroy();
}

void GameModeSelectorLoadingComponent::BeginPlay()
{

	dae::Scene& scene = dae::SceneManager::GetInstance().GetCurrentScene();

	auto tank = std::make_unique<dae::GameObject>();
	m_Selector = tank.get();
	
	tank->AddComponent<PhysicsComponent>(glm::vec2{ 20.f, 20.f });
	auto playerControllerComponentKeyboard = tank->AddComponent<PlayerControllerComponent>(100.f);
	auto selectorRenderer = tank->AddComponent<RenderComponentEx>(20.f, 20.f);
	selectorRenderer->SetTexture("Textures/T_Selector.png");
	tank->SetPosition(dae::Minigin::windowWidth/2 - 20.f/2, dae::Minigin::windowHeight/2 - 20.f/2);
	scene.Add(std::move(tank));


	auto inputMappingKeyboard = new InputMapping();
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_W, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,-1 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_D, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_S, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_A, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
	dae::InputManager::GetInstance().GetPlayerController(-1)->AddMapping(inputMappingKeyboard);
	

	auto inputMappingController = new InputMapping();
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_UP, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,-1 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_RIGHT, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_DOWN, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_LEFT, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
	dae::InputManager::GetInstance().GetPlayerController(0)->AddMapping(inputMappingController);

	ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("GameIntro"), 50.f);

}
