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

using json = nlohmann::json;

std::unique_ptr<Level> JSONLevelLoader::Parse(const std::string& filePath, dae::Scene& scene)
{
		
		std::unique_ptr<Level> level = std::make_unique<Level>();

		std::filesystem::path path{ filePath };
		std::ifstream file{ path };
			
		//assert(file.is_open() && "Failed to open level!");

		json levelJson;
		file >> levelJson;

		dae::GameObject* gridObject = new dae::GameObject();
		GridComponent* gridComponent = gridObject->AddComponent<GridComponent>(static_cast<float>(dae::Minigin::windowWidth), static_cast<float>(dae::Minigin::windowHeight), 30, 30);
		level->m_GridObject = gridObject;
		scene.Add(level->m_GridObject);

		float collisionTileWidth = dae::Minigin::windowWidth / 30.f;
		float collisionTileHeight = dae::Minigin::windowHeight / 30.f;
		if (levelJson.contains("collission_tiles") && levelJson["collission_tiles"].is_array()) {
		
			for (const auto& tile : levelJson["collission_tiles"]) {
				int x = tile.at("x");
				int y = tile.at("y");

				dae::GameObject* gameobject = new dae::GameObject();
				gameobject->AddComponent<PhysicsComponent>(glm::vec2{ collisionTileWidth,collisionTileHeight })->SetIsStatic(true);
				scene.Add(gameobject);
				gridComponent->InsertAndParent(gameobject, x, y);
			}


			int numFillTiles = (30*30)-int(levelJson["collission_tiles"].size());
			for (int fillTile{}; fillTile < numFillTiles; ++fillTile)
			{
				dae::GameObject* gameobject = new dae::GameObject();
				gameobject->AddComponent<RectangleRendererComponent>(collisionTileWidth+1, collisionTileHeight+1, "Textures/T_FillTile.png");
				scene.Add(gameobject);
				gameobject->SetParent(gridObject, false);
			}

		}

		dae::GameObject* gridPathObject = new dae::GameObject();
		gridPathObject->AddComponent<GridPathRenderComponent>(gridComponent);
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
				enemy->AddComponent<AITankComponent>(gridComponent, &level->m_Players, 30.f);
				auto renderer  = enemy->AddComponent<TankRendererComponent>("Textures/T_EnemyTank.png", 40.f, 40.f);
				enemy->GetGameObjectEventDispatcher()->AddObserver(renderer);
				glm::vec2 positionInWorld = gridComponent->GetPositionAt(spawnpoint.at("x"), spawnpoint.at("y"));
				enemy->SetPosition(positionInWorld.x, positionInWorld.y);
				scene.Add(enemy);
			}
		}


		return level;

}
