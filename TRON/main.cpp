
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
	dae::SceneManager::GetInstance().SelectScene("Level");
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<RectangleRendererComponent>((float)dae::Minigin::windowWidth, (float)dae::Minigin::windowHeight,"Textures/T_CircuitBoard.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto text = go->AddComponent<TextRenderComponent>("FPS: ", "Lingua.otf", 11);
	go->AddComponent<FPSComponent>(text);
	scene.Add(std::move(go));     

	JSONLevelLoader loader{};
	loader.Parse("../Data/Levels/LevelUno.json", scene);

	loader.GetLevel().m_GridObject->GetComponent<GridComponent>()->GetGraphFromGrid();

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