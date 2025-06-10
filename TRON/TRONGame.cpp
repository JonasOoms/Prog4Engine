#include "TRONGame.h"
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
#include "GamePrefabs.h"
#include "BounceyPhysicsComponent.h"
#include "GameRegistries.h"
#include "LevelManagerComponent.h"
#include "GameModeSelectorLoadingComponent.h"
#include "ActivationBoxComponent.h"
#include "ScoreManagerComponent.h"
#include "BenchmarkPhysicsSystem.h"

TRONGame::TRONGame():
	m_HighScores{ "../Data/SaveGame/TronScores.json" }
{
}

TRONGame::~TRONGame()
{
	m_HighScores.Serialize("../Data/SaveGame/TronScores.json");
}

void TRONGame::LoadMainMenu()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Menu");
	scene.RemoveAll();
	dae::SceneManager::GetInstance().SelectScene("Menu");


	constexpr float width = 500.f;
	constexpr float height = 350.f;



	auto go = std::make_unique<dae::GameObject>();
	auto menuSelectorSprite = go->AddComponent<RenderComponentEx>(width, height);
	menuSelectorSprite->SetTexture("Textures/T_MenuSelector.png");
	go->SetPosition(dae::Minigin::windowWidth / 2 - width / 2, dae::Minigin::windowHeight / 2 - height / 2);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto text = go->AddComponent<TextRenderComponent>("FPS: ", "Fonts/Volter_Goldfish.ttf", 11);
	text->SetColor(SDL_Color{ 255,255,255,100 });
	go->AddComponent<FPSComponent>(text);
	scene.Add(std::move(go));



	go = std::make_unique<dae::GameObject>();
	auto titleScreenSprite = go->AddComponent<RenderComponentEx>(width, height/3);
	titleScreenSprite->SetTexture("Textures/T_Logo.png");
	go->SetPosition(dae::Minigin::windowWidth / 2 - width / 2, dae::Minigin::windowHeight / 2 - height/1.15f);
	scene.Add(std::move(go));


	go = std::make_unique<dae::GameObject>();
	auto textVersusMode = go->AddComponent<TextRenderComponent>("VS", "Fonts/Volter_Goldfish.ttf", 44);
	textVersusMode->SetColor(SDL_Color{ 255,255,255,255 });
	go->SetPosition(dae::Minigin::windowWidth / 2 - 170, dae::Minigin::windowHeight / 2 - 11 );
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto textCoOpMode = go->AddComponent<TextRenderComponent>("Co-Op", "Fonts/Volter_Goldfish.ttf", 44);
	textCoOpMode->SetColor(SDL_Color{ 255,255,255,255 });
	go->SetPosition(dae::Minigin::windowWidth / 2 + 75, dae::Minigin::windowHeight / 2 - 11);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto textSingleplayer = go->AddComponent<TextRenderComponent>("Singleplayer", "Fonts/Volter_Goldfish.ttf", 33);
	textSingleplayer->SetColor(SDL_Color{ 255,255,255,255 });
	go->SetPosition(dae::Minigin::windowWidth / 2 - 33*3.3f, dae::Minigin::windowHeight / 2 - 100);
	scene.Add(std::move(go));


	go = std::make_unique<dae::GameObject>();
	auto singlePlayerBox = go->AddComponent<ActivationBoxComponent>(LoadSingleplayerLevel);
	go->AddComponent<PhysicsComponent>(glm::vec2{ 130,100 });
	go->SetPosition(dae::Minigin::windowWidth / 2 - 130 / 2 - 20, dae::Minigin::windowHeight / 2 - 130);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto coopBox = go->AddComponent<ActivationBoxComponent>(LoadCoopLevel);
	go->AddComponent<PhysicsComponent>(glm::vec2{ 130,100 });
	go->SetPosition(dae::Minigin::windowWidth / 2 + 75, dae::Minigin::windowHeight / 2 - 11);
	scene.Add(std::move(go));


	auto tank = std::make_unique<dae::GameObject>();

	tank->AddComponent<PhysicsComponent>(glm::vec2{ 20.f, 20.f });
	tank->AddComponent<PlayerControllerComponent>(100.f);
	auto selectorRenderer = tank->AddComponent<RenderComponentEx>(20.f, 20.f);
	selectorRenderer->SetTexture("Textures/T_Selector.png");
	tank->SetPosition(dae::Minigin::windowWidth / 2 - 20.f / 2, dae::Minigin::windowHeight / 2 - 20.f / 2);


	auto activationBoxes = std::vector{ singlePlayerBox, coopBox };
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<GameModeSelectorLoadingComponent>(this, tank.get(), activationBoxes);
	scene.Add(std::move(go));

	scene.Add(std::move(tank));

}

void TRONGame::LoadLevel(GameMode gamemode)
{

	m_SelectedGameMode = gamemode;
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level");
	scene.RemoveAll();
	dae::SceneManager::GetInstance().SelectScene("Level");

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<RectangleRendererComponent>((float)dae::Minigin::windowWidth, (float)dae::Minigin::windowHeight, "Textures/T_CircuitBoard.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto text = go->AddComponent<TextRenderComponent>("FPS: ", "Fonts/Volter_Goldfish.ttf", 11);
	go->AddComponent<FPSComponent>(text);
	scene.Add(std::move(go));

	auto HUDBlackout = std::make_unique<dae::GameObject>();
	HUDBlackout->AddComponent<RenderComponentEx>((float)dae::Minigin::windowWidth, 65.f)->SetTexture("Textures/T_FillTile.png");
	scene.Add(std::move(HUDBlackout));

	auto LifeCounter = std::make_unique <dae::GameObject>();
	auto lifeCounterText = LifeCounter->AddComponent<TextRenderComponent>("1st", "Fonts/Volter_Goldfish.ttf", 20);
	lifeCounterText->SetColor(SDL_Color{ 0,0,255,255 });
	LifeCounter->SetPosition(30, 10);
	scene.Add(std::move(LifeCounter));

	auto ScoreCounter = std::make_unique <dae::GameObject>();
	auto scoreText = ScoreCounter->AddComponent<TextRenderComponent>("0", "Fonts/Volter_Goldfish.ttf", 20);
	scoreText->SetColor(SDL_Color{ 0,0,255,255 });
	ScoreCounter->SetPosition(70, 34);
	scene.Add(std::move(ScoreCounter));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<LevelManagerComponent>(this, scoreText, lifeCounterText ,nullptr);
	scene.Add(std::move(go));

	
}

void TRONGame::LoadScoreScreen()
{

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Scores");
	scene.RemoveAll();
	dae::SceneManager::GetInstance().SelectScene("Scores");
	auto& inputManager = dae::InputManager::GetInstance();
	inputManager.ClearAllMappings();


	auto go = std::make_unique<dae::GameObject>();
	auto scoreManager = go->AddComponent<ScoreManagerComponent>(this);
	scene.Add(std::move(go));
	
	auto inputMappingKeyboard = std::make_unique<InputMapping>();
	inputMappingKeyboard->AddInputBinding(SDLK_ESCAPE, TriggerType::Released, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Back));
	inputMappingKeyboard->AddInputBinding(SDLK_RETURN, TriggerType::Released, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Enter));
	inputMappingKeyboard->AddInputBinding(SDLK_LEFT, TriggerType::Released, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Left));
	inputMappingKeyboard->AddInputBinding(SDLK_RIGHT, TriggerType::Released, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Right));
	inputMappingKeyboard->AddInputBinding(SDLK_UP, TriggerType::Released, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Up));
	inputMappingKeyboard->AddInputBinding(SDLK_DOWN, TriggerType::Released, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Down));
	inputManager.GetPlayerController(-1)->AddMapping(std::move(inputMappingKeyboard));

	auto inputMappingController = std::make_unique<InputMapping>();
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_B, TriggerType::Down, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Back));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_A, TriggerType::Down, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Enter));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_LEFT, TriggerType::Down, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Left));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_RIGHT, TriggerType::Down, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Right));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_UP, TriggerType::Down, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Up));
	inputMappingController->AddInputBinding(XINPUT_GAMEPAD_DPAD_DOWN, TriggerType::Down, std::make_unique<ScoreManagerCommand>(scoreManager, ScoreManagerAction::Down));
	inputManager.GetPlayerController(0)->AddMapping(std::move(inputMappingController));
	
	

}



void TRONGame::Load()
{

		ServiceLocator::RegisterSoundSystem(std::make_unique<SDL_SoundSystem>());
		ServiceLocator::RegisterPhysicsSystem(std::make_unique<BenchmarkPhysicsSystem>(std::move(std::make_unique<SimpleSpatialPhysicsSystem>((float)dae::Minigin::windowWidth, (float)dae::Minigin::windowHeight))));


		auto soundSystem = ServiceLocator::GetSoundSystem();
		TRONRegistries::GameSoundRegistry.Register("GameIntro", soundSystem->RegisterAudio("../Data/Sounds/S_GameIntro.wav"));
		TRONRegistries::GameSoundRegistry.Register("LevelIntro", soundSystem->RegisterAudio("../Data/Sounds/S_Intro.wav"));
		TRONRegistries::GameSoundRegistry.Register("SFX1", soundSystem->RegisterAudio("../Data/Sounds/S_SFX1.wav"));
		TRONRegistries::GameSoundRegistry.Register("Explosion", soundSystem->RegisterAudio("../Data/Sounds/S_Explosion.wav"));

		TRONRegistries::GameSoundRegistry.Register("Back", soundSystem->RegisterAudio("../Data/Sounds/S_Back.wav"));
		TRONRegistries::GameSoundRegistry.Register("Select", soundSystem->RegisterAudio("../Data/Sounds/S_Select.wav"));
		TRONRegistries::GameSoundRegistry.Register("Save", soundSystem->RegisterAudio("../Data/Sounds/S_Saved.wav"));

		LoadMainMenu();

		dae::SceneManager::GetInstance().SelectScene("Menu");
		//dae::SceneManager::GetInstance().SelectScene("Level");
	
		

}

std::string_view TRONGame::GetGameTitle()
{
	return "TRON Arcade - Ooms Jonas 2GD11";
}

void TRONGame::ChangeGameMode(GameMode gamemode)
{
	//assert(!m_IsInLevel && "You cannot change gamemode inside of the level!");
	m_SelectedGameMode = gamemode;
}
