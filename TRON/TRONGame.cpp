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

TRONGame::TRONGame()
{
	// deserialize high scores
}

TRONGame::~TRONGame()
{
	// serialize high scores
}

void TRONGame::LoadMainMenu()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Menu");
	
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
	auto singlePlayerBox = go.get();
	go->AddComponent<ActivationBoxComponent>([&]()
		{
			LoadLevel();
			this->ChangeGameMode(GameMode::Singleplayer);
			dae::SceneManager::GetInstance().SelectScene("Level");

		});
	go->AddComponent<PhysicsComponent>(glm::vec2{ 130,100 });
	go->SetPosition(dae::Minigin::windowWidth / 2 - 130 / 2 - 20, dae::Minigin::windowHeight / 2 - 130);
	scene.Add(std::move(go));

	auto tank = std::make_unique<dae::GameObject>();

	tank->AddComponent<PhysicsComponent>(glm::vec2{ 20.f, 20.f });
	tank->AddComponent<PlayerControllerComponent>(100.f);
	auto selectorRenderer = tank->AddComponent<RenderComponentEx>(20.f, 20.f);
	selectorRenderer->SetTexture("Textures/T_Selector.png");
	tank->SetPosition(dae::Minigin::windowWidth / 2 - 20.f / 2, dae::Minigin::windowHeight / 2 - 20.f / 2);


	go = std::make_unique<dae::GameObject>();
	go->AddComponent<GameModeSelectorLoadingComponent>(this, tank.get(), singlePlayerBox);
	scene.Add(std::move(go));

	scene.Add(std::move(tank));


}

void TRONGame::LoadLevel()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level");
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



void TRONGame::Load()
{

		ServiceLocator::RegisterSoundSystem(std::make_unique<SDL_SoundSystem>());
		ServiceLocator::RegisterPhysicsSystem(std::make_unique<SimpleSpatialPhysicsSystem>((float)dae::Minigin::windowWidth, (float)dae::Minigin::windowHeight));


		auto soundSystem = ServiceLocator::GetSoundSystem();
		TRONRegistries::GameSoundRegistry.Register("GameIntro", soundSystem->RegisterAudio("../Data/Sounds/S_GameIntro.wav"));
		TRONRegistries::GameSoundRegistry.Register("LevelIntro", soundSystem->RegisterAudio("../Data/Sounds/S_Intro.wav"));
		TRONRegistries::GameSoundRegistry.Register("SFX1", soundSystem->RegisterAudio("../Data/Sounds/S_SFX1.wav"));
		TRONRegistries::GameSoundRegistry.Register("Explosion", soundSystem->RegisterAudio("../Data/Sounds/S_Explosion.wav"));

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
	assert(!m_IsInLevel && "You cannot change gamemode inside of the level!");
	m_SelectedGameMode = gamemode;
}
