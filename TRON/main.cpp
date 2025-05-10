
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include <iostream>
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TankRendererComponent.h"
#include "TextRenderComponent.h"
#include "FPSComponent.h"
#include "PlayerControllerComponent.h"
#include "ValueComponent.h"
#include "ValueDisplayComponent.h"
#include "InputManager.h"
#include "PlayerControllerMoveCommand.h"
#include "ValueIncreaseCommand.h"
#include "AchievementObserver.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Xinput.h"
#include "ServiceLocator.h"
#include "SDL_SoundSystem.h"
#include "TurretComponent.h"

#include "SimpleSpatialPhysicsSystem.h"
#include <memory>
#include "GridComponent.h"
#include "RectangleRenderer.h"
#include "LevelLoader.h"
#include "GamePrefabs.h"
#include "BounceyPhysicsComponent.h"
#include "GameRegistries.h"

void load()
{

	ServiceLocator::RegisterSoundSystem(std::make_unique<SDL_SoundSystem>());
	ServiceLocator::RegisterPhysicsSystem(std::make_unique<SimpleSpatialPhysicsSystem>((float) dae::Minigin::windowWidth, (float) dae::Minigin::windowHeight));

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level");

	auto go = new dae::GameObject();
	go->AddComponent<RectangleRendererComponent>((float)dae::Minigin::windowWidth, (float)dae::Minigin::windowHeight,"Textures/T_CircuitBoard.png");
	scene.Add(go);

	JSONLevelLoader loader{};
	loader.Parse("../Data/Levels/LevelUno.json", scene);
	
	//TRONGameObjects::PrefabFactory{}.CreatePlayerTank(scene, 0);
	//auto playerTankRendererComponent = tank->AddComponent<TankRendererComponent>("Textures/T_PlayerTank.png", 40.f, 40.f);
	//tank->AddComponent<PhysicsComponent>(glm::vec2{ 40.f, 40.f });
	//auto playerControllerComponentKeyboard = tank->AddComponent<PlayerControllerComponent>(200.f);
	//playerControllerComponentKeyboard->GetMovedEvent()->AddObserver(playerTankRendererComponent);


	///*auto keyboardCharacterLives = keyboardCharacterObject->AddComponent<ValueComponent<int>>(3);
	//auto keyboardCharacterPoints = keyboardCharacterObject->AddComponent<ValueComponent<int>>(0);*/
	//tank->SetPosition(100, 100);
	//scene.Add(tank);

	//auto playerTankTurretObject = new dae::GameObject();
	//playerTankTurretObject->AddComponent<TurretComponent>("Textures/T_Turret.png", 60.f, 60.f);
	//playerTankTurretObject->SetParent(tank, false);
	//playerTankTurretObject->SetPosition(-8, -8);
	//scene.Add(playerTankTurretObject);


	//auto inputMappingKeyboard = new InputMapping();
	//inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_W, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,-1 }));
	//inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_D, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
	//inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_S, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
	//inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_A, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
	//dae::InputManager::GetInstance().GetPlayerController(-1)->AddMapping(inputMappingKeyboard);


	//auto inputMapping = new InputMapping();
	//inputMapping->AddInputBinding(XINPUT_GAMEPAD_DPAD_UP, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,-1 }));
	//inputMapping->AddInputBinding(XINPUT_GAMEPAD_DPAD_RIGHT, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
	//inputMapping->AddInputBinding(XINPUT_GAMEPAD_DPAD_DOWN, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
	//inputMapping->AddInputBinding(XINPUT_GAMEPAD_DPAD_LEFT, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
	//dae::InputManager::GetInstance().GetPlayerController(0)->AddMapping(inputMapping);


	auto soundSystem = ServiceLocator::GetSoundSystem();
	TRONRegistries::GameSoundRegistry.Register("Intro", soundSystem->RegisterAudio("../Data/Sounds/S_Intro.wav"));
	TRONRegistries::GameSoundRegistry.Register("SFX1", soundSystem->RegisterAudio("../Data/Sounds/S_SFX1.wav"));

	

	
	soundSystem->Play(TRONRegistries::GameSoundRegistry.Get("Intro"), 128.f);
	
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	// enable for steam support
	//SteamAPI_Shutdown();
	return 0;
}