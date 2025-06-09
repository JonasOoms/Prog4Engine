#include "LevelLoader.h"
#include <json.hpp>
#include <fstream>
#include "GridComponent.h"
#include "Minigin.h"
#include "GamePrefabs.h"
#include "RectangleRenderer.h"
#include "GridPathRenderer.h"
#include "glm.hpp"
#include "AITankComponent.h"
#include "AiRecogniserComponent.h"

using json = nlohmann::json;

std::unique_ptr<Level> JSONLevelLoader::LoadSinglePlayer(const std::string& filePath, dae::Scene& scene)
{
	return std::move(Parse(filePath, scene));
}

std::unique_ptr<Level> JSONLevelLoader::LoadCoOp(const std::string& filePath, dae::Scene& scene)
{
	std::unique_ptr<Level> level = std::move(Parse(filePath, scene));

	std::filesystem::path path{ filePath };
	std::ifstream file{ path };

	json levelJson;
	file >> levelJson;

	//dae::InputManager::GetInstance().GetPlayerController(0)->ClearMapping();
	auto gridComponent = level->m_GridObject->GetComponent<GridComponent>();

	auto spawnpoint = levelJson["player_spawnpoint"];
	dae::GameObject* player = TRONGameObjects::PrefabFactory{}.CreatePlayerTank(scene, 1);
	glm::vec2 positionInWorld = gridComponent->GetPositionAt(static_cast<int>(spawnpoint.at("x")), static_cast<int>(spawnpoint.at("y")));
	player->SetPosition(positionInWorld.x, positionInWorld.y);
	level->m_Players.emplace_back(player);

	return level;
}

std::unique_ptr<Level> JSONLevelLoader::LoadVS(const std::string& , dae::Scene&)
{
	return std::unique_ptr<Level>();
}

std::unique_ptr<Level> JSONLevelLoader::Parse(const std::string& filePath, dae::Scene& scene)
{
		
		std::unique_ptr<Level> level = std::make_unique<Level>();

		std::filesystem::path path{ filePath };
		std::ifstream file{ path };
			
		//assert(file.is_open() && "Failed to open level!");

		json levelJson;
		file >> levelJson;

		dae::GameObject* gridObject = new dae::GameObject();
		GridComponent* gridComponent = gridObject->AddComponent<GridComponent>(static_cast<float>(dae::Minigin::windowWidth), static_cast<float>(dae::Minigin::windowHeight - 40), 30, 30);
		gridObject->SetPosition(0, 50);
		level->m_GridObject = gridObject;
		scene.Add(level->m_GridObject);

		float collisionTileWidth = dae::Minigin::windowWidth / 30.f;
		float collisionTileHeight = (dae::Minigin::windowHeight - 50) / 30.f;

		if (levelJson.contains("collission_tiles") && levelJson["collission_tiles"].is_array()) {
			auto& tiles = levelJson["collission_tiles"];
			size_t tileSearch{};
			for (int row{}; row < 30; ++row)
			{
				for (int column{}; column < 30; ++column)
				{
					// get the collision tile we are currently searching for
					auto& selectedCollisionTile = tiles[tileSearch];
					

					// Is the tile we're currently on the collision tile?
					if (selectedCollisionTile["y"] == column && selectedCollisionTile["x"] == row)
					{
						// we found our collision tile and make an object for it!
						dae::GameObject* gameobject = new dae::GameObject();
						scene.Add(gameobject);
						gameobject->SetParent(gridObject, false);
						gameobject->AddComponent<PhysicsComponent>(glm::vec2{ collisionTileWidth ,collisionTileHeight })->SetIsStatic(true);
						level->m_CollisionTiles.emplace_back(gameobject);
						++tileSearch;
					}
					else
					{
						// we have not found our collision tile and we make a fill tile!
						dae::GameObject* gameobject = new dae::GameObject();
						gameobject->AddComponent<RectangleRendererComponent>(collisionTileWidth + 1, collisionTileHeight + 1, "Textures/T_FillTile.png");
						scene.Add(gameobject);
						gameobject->SetParent(gridObject, false);
					}


				}
			}
		}



		dae::GameObject* gridPathObject = new dae::GameObject();
		gridPathObject->AddComponent<GridPathRenderComponent>(gridComponent);
		level->m_GridPathRenderer = gridPathObject;
		scene.Add(gridPathObject);

		if (levelJson.contains("player_spawnpoint"))
		{
			auto spawnpoint = levelJson["player_spawnpoint"];

			dae::GameObject* player = TRONGameObjects::PrefabFactory{}.CreatePlayerTank(scene, 0);
			glm::vec2 positionInWorld = gridComponent->GetPositionAt(spawnpoint.at("x"), spawnpoint.at("y"));
			player->SetPosition(positionInWorld.x, positionInWorld.y);
			level->m_Players.emplace_back(player);

		}

		if (levelJson.contains("enemy_tanks") && levelJson["enemy_tanks"].is_array())
		{
			for (const auto& spawnpoint : levelJson["enemy_tanks"])
			{
				dae::GameObject* enemy = new dae::GameObject();
				enemy->AddComponent<PhysicsComponent>(glm::vec2{ 40.f,40.f });
				enemy->AddComponent<AITankComponent>(gridComponent, &level->m_Players, 60.f);
				auto renderer  = enemy->AddComponent<TankRendererComponent>("Textures/T_EnemyTank.png", 40.f, 40.f);
				enemy->GetGameObjectEventDispatcher()->AddObserver(renderer);
				glm::vec2 positionInWorld = gridComponent->GetPositionAt(spawnpoint.at("x"), spawnpoint.at("y"));
				enemy->SetPosition(positionInWorld.x, positionInWorld.y);
				level->m_Enemies.emplace_back(enemy);
				scene.Add(enemy);

				

			}
		}

		if (levelJson.contains("enemy_recognizers") && levelJson["enemy_recognizers"].is_array())
		{
			for (const auto& spawnpoint : levelJson["enemy_recognizers"])
			{
				dae::GameObject* enemy = new dae::GameObject();
				glm::vec2 size{ 40.f,40.f };
				enemy->AddComponent<RecogniserCollisionComponent>(size);
				enemy->AddComponent<AIRecogniserComponent>(gridComponent, &level->m_Players, 120.f);
				auto renderer = enemy->AddComponent<TankRendererComponent>("Textures/T_Recogniser.png", 40.f, 40.f);
				enemy->GetGameObjectEventDispatcher()->AddObserver(renderer);
				glm::vec2 positionInWorld = gridComponent->GetPositionAt(spawnpoint.at("x"), spawnpoint.at("y"));
				enemy->SetPosition(positionInWorld.x, positionInWorld.y);
				level->m_Recognisers.emplace_back(enemy);
				scene.Add(enemy);

			}
		}


		return level;

}
