#include "LevelLoader.h"
#include <json.hpp>
#include <fstream>
#include "GridComponent.h"
#include "StaticPhysicsComponent.h"
#include "Minigin.h"
#include "GamePrefabs.h"
#include "RectangleRenderer.h"
#include "glm.hpp"

using json = nlohmann::json;

void JSONLevelLoader::Parse(const std::string& filePath, dae::Scene& scene)
{

		Level level{};

		std::filesystem::path path{ filePath };
		std::ifstream file{ path };
			
		//assert(file.is_open() && "Failed to open level!");

		json levelJson;
		file >> levelJson;

		dae::GameObject* gridObject = new dae::GameObject();
		GridComponent* gridComponent = gridObject->AddComponent<GridComponent>(static_cast<float>(dae::Minigin::windowWidth), static_cast<float>(dae::Minigin::windowHeight), 30, 30);
		level.m_GridObject = gridObject;
		scene.Add(level.m_GridObject);

		float collisionTileWidth = dae::Minigin::windowWidth / 30.f;
		float collisionTileHeight = dae::Minigin::windowHeight / 30.f;

		if (levelJson.contains("collission_tiles") && levelJson["collission_tiles"].is_array()) {
		
			for (const auto& tile : levelJson["collission_tiles"]) {
				int x = tile.at("x");
				int y = tile.at("y");

				dae::GameObject* gameobject = new dae::GameObject();
				gameobject->AddComponent<StaticPhysicsComponent>(glm::vec2{ collisionTileWidth,collisionTileHeight });
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

		if (levelJson.contains("player_spawnpoint"))
		{
			auto spawnpoint = levelJson["player_spawnpoint"];

			level.m_Player = TRONGameObjects::PrefabFactory{}.CreatePlayerTank(scene, 0);
			glm::vec2 positionInWorld = gridComponent->GetPositionAt(spawnpoint.at("x"), spawnpoint.at("y"));
			level.m_Player->SetPosition(positionInWorld.x, positionInWorld.y);

		}

		m_Level = level;

}
