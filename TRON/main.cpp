#include <SDL.h>

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

void LoadDemoScene(dae::Scene& scene)
{
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<RenderComponent>()->SetTexture("Background.tga");
	scene.Add(go);


	go = std::make_shared<dae::GameObject>();
	auto component = go->AddComponent<RenderComponent>();
	component->SetTexture("Logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);


	go = std::make_shared<dae::GameObject>();
	go->AddComponent<TextRenderComponent>("Programming 4 Assignment", "Lingua.otf", 36);
	go->SetPosition(80, 20);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	TextRenderComponent* textComponent = go->AddComponent<TextRenderComponent>("FPS", "Lingua.otf", 22);
	go->AddComponent<FPSComponent>(textComponent);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<TextRenderComponent>("Use the D-Pad to move the red tank, X to inflict damage, A and B to pick up pellets", "Lingua.otf", 14);
	go->SetPosition(10, 100);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<TextRenderComponent>("Use WASD to move the blue tank, C to inflict damage, Z and X to pick up pellets", "Lingua.otf", 14);
	go->SetPosition(10, 120);
	scene.Add(go);


	auto keyboardCharacterObject = std::make_shared<dae::GameObject>();
	auto playerTankRendererComponent = keyboardCharacterObject->AddComponent<TankRendererComponent>("Textures/T_PlayerTank.png",  45.f,45.f);
	auto playerControllerComponentKeyboard = keyboardCharacterObject->AddComponent<PlayerControllerComponent>(30.f);
	playerControllerComponentKeyboard->GetMovedEvent()->AddObserver(playerTankRendererComponent);

	auto keyboardCharacterLives = keyboardCharacterObject->AddComponent<ValueComponent<int>>(3);
	auto keyboardCharacterPoints = keyboardCharacterObject->AddComponent<ValueComponent<int>>(0);
	keyboardCharacterObject->SetPosition(100, 100);
	scene.Add(keyboardCharacterObject);

	auto gamepadCharacterObject = std::make_shared<dae::GameObject>();
	gamepadCharacterObject->AddComponent<RenderComponent>()->SetTexture("Textures/T_EnemyTank.png");
	auto gamepadCharacterLives = gamepadCharacterObject->AddComponent<ValueComponent<int>>(3);
	auto gamepadCharacterPoints = gamepadCharacterObject->AddComponent<ValueComponent<int>>(0);
	auto playerControllerComponentGamepad = gamepadCharacterObject->AddComponent<PlayerControllerComponent>(30.f);
	gamepadCharacterObject->SetPosition(30, 100);
	scene.Add(gamepadCharacterObject);


	go = std::make_shared<dae::GameObject>();
	auto livesDisplay1 = go->AddComponent<ValueDisplayComponent>("#Lives: ", "", keyboardCharacterLives->Get());
	keyboardCharacterLives->AddObserver(livesDisplay1);
	go->SetPosition(10, 200);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto pointDisplay1 = go->AddComponent<ValueDisplayComponent>("#Points: ", "", keyboardCharacterPoints->Get());
	gamepadCharacterPoints->AddObserver(pointDisplay1);
	go->SetPosition(10, 220);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto livesDisplay2 = go->AddComponent<ValueDisplayComponent>("#Lives: ", "", keyboardCharacterLives->Get());
	gamepadCharacterLives->AddObserver(livesDisplay2);
	go->SetPosition(10, 240);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto pointDisplay2 = go->AddComponent<ValueDisplayComponent>("#Points: ", "", keyboardCharacterPoints->Get());
	keyboardCharacterPoints->AddObserver(pointDisplay2);
	go->SetPosition(10, 260);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	AchievementObserver* achievement1 = go->AddComponent<AchievementObserver>();
	gamepadCharacterPoints->AddObserver(achievement1);
	AchievementObserver* achievement2 = go->AddComponent<AchievementObserver>();
	keyboardCharacterPoints->AddObserver(achievement2);
	scene.Add(go);


	auto inputMappingKeyboard = new InputMapping();
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_W, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,-1 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_D, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_S, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
	inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_A, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));

	inputMappingKeyboard->AddInputBinding(SDLK_c, TriggerType::Released, new ValueIncreaseCommand<int>(-1, keyboardCharacterLives));
	inputMappingKeyboard->AddInputBinding(SDLK_z, TriggerType::Released, new ValueIncreaseCommand<int>(10, keyboardCharacterPoints));
	inputMappingKeyboard->AddInputBinding(SDLK_x, TriggerType::Released, new ValueIncreaseCommand<int>(100, keyboardCharacterPoints));

	dae::InputManager::GetInstance().GetPlayerController(-1)->AddMapping(inputMappingKeyboard);

	auto inputMappingGamepad = new InputMapping();
	inputMappingGamepad->AddInputBinding(XINPUT_GAMEPAD_DPAD_UP, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentGamepad, glm::vec2{ 0,-1 }));
	inputMappingGamepad->AddInputBinding(XINPUT_GAMEPAD_DPAD_RIGHT, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentGamepad, glm::vec2{ 1,0 }));
	inputMappingGamepad->AddInputBinding(XINPUT_GAMEPAD_DPAD_DOWN, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentGamepad, glm::vec2{ 0,1 }));
	inputMappingGamepad->AddInputBinding(XINPUT_GAMEPAD_DPAD_LEFT, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentGamepad, glm::vec2{ -1,0 }));

	inputMappingGamepad->AddInputBinding(XINPUT_GAMEPAD_X, TriggerType::Released, new ValueIncreaseCommand<int>(-1, keyboardCharacterLives));
	inputMappingGamepad->AddInputBinding(XINPUT_GAMEPAD_A, TriggerType::Released, new ValueIncreaseCommand<int>(10, keyboardCharacterPoints));
	inputMappingGamepad->AddInputBinding(XINPUT_GAMEPAD_B, TriggerType::Released, new ValueIncreaseCommand<int>(100, keyboardCharacterPoints));

	dae::InputManager::GetInstance().GetPlayerController(0)->AddMapping(inputMappingGamepad);
}

void load()
{

	ServiceLocator::RegisterSoundSystem(std::make_unique<SDL_SoundSystem>());

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	LoadDemoScene(scene);

	auto soundSystem = ServiceLocator::GetSoundSystem();
	sound_id sound1 = soundSystem->RegisterAudio("../Data/Sounds/S_Intro.wav");
	soundSystem->RegisterAudio("../Data/Sounds/S_SFX1.wav");
	
	soundSystem->Play(sound1, 128.f);
	

}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	// enable for steam support
	//SteamAPI_Shutdown();
	return 0;
}