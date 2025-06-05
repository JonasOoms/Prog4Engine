#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Xinput.h"
#include "GameObject.h"
#include "TankRendererComponent.h"
#include "PhysicsComponent.h"
#include "PlayerControllerComponent.h"
#include "TurretComponent.h"
#include "Scene.h"
#include "InputMapping.h"
#include "PlayerControllerMoveCommand.h"
#include "InputManager.h"
#include "TurretAngleChangeCommand.h"
#include "BounceyPhysicsComponent.h"
#include "RenderComponentEx.h"
#include "ShootCommand.h"

namespace TRONGameObjects
{
	class PrefabFactory final
	{
	public:
		inline dae::GameObject* CreatePlayerTank(dae::Scene& scene, int playerId)
		{
			auto tank = std::make_unique<dae::GameObject>();
			auto tankPointer = tank.get();
			auto playerTankRendererComponent = tank->AddComponent<TankRendererComponent>("Textures/T_PlayerTank.png", 40.f, 40.f);
			tank->AddComponent<PhysicsComponent>(glm::vec2{ 40.f, 40.f });
			auto playerControllerComponentKeyboard = tank->AddComponent<PlayerControllerComponent>(100.f);
			playerControllerComponentKeyboard->GetMovedEvent()->AddObserver(playerTankRendererComponent);

			tank->SetPosition(100, 100);
			scene.Add(std::move(tank));

			auto playerTankTurretObject = std::make_unique<dae::GameObject>();
			auto turret = playerTankTurretObject->AddComponent<TurretComponent>("Textures/T_Turret.png", 60.f, 60.f);
			playerTankTurretObject->SetParent(tankPointer, false);
			playerTankTurretObject->SetPosition(-8, -8);
			scene.Add(std::move(playerTankTurretObject));

			if (playerId == 0)
			{
				auto inputMappingKeyboard = new InputMapping();
				inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_W, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,-1 }));
				inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_D, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
				inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_S, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
				inputMappingKeyboard->AddInputBinding(SDL_SCANCODE_A, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
				inputMappingKeyboard->AddInputBinding(SDLK_q, TriggerType::Down, new TurretAngleChangeCommand(turret, -30));
				inputMappingKeyboard->AddInputBinding(SDLK_e, TriggerType::Down, new TurretAngleChangeCommand(turret, 30));
				inputMappingKeyboard->AddInputBinding(SDLK_SPACE, TriggerType::Down, new ShootCommand(turret, scene, 15, 0.5f));
				dae::InputManager::GetInstance().GetPlayerController(-1)->AddMapping(inputMappingKeyboard);
			}

			auto inputMappingKeyboard = new InputMapping();
			inputMappingKeyboard->AddInputBinding(XINPUT_GAMEPAD_DPAD_UP, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,-1 }));
			inputMappingKeyboard->AddInputBinding(XINPUT_GAMEPAD_DPAD_RIGHT, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 1,0 }));
			inputMappingKeyboard->AddInputBinding(XINPUT_GAMEPAD_DPAD_DOWN, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ 0,1 }));
			inputMappingKeyboard->AddInputBinding(XINPUT_GAMEPAD_DPAD_LEFT, TriggerType::Pressed, new PlayerControllerMoveCommand(playerControllerComponentKeyboard, glm::vec2{ -1,0 }));
			inputMappingKeyboard->AddInputBinding(XINPUT_GAMEPAD_LEFT_SHOULDER, TriggerType::Down, new TurretAngleChangeCommand(turret, -30));
			inputMappingKeyboard->AddInputBinding(XINPUT_GAMEPAD_RIGHT_SHOULDER, TriggerType::Down, new TurretAngleChangeCommand(turret, 30));
			inputMappingKeyboard->AddInputBinding(XINPUT_GAMEPAD_A, TriggerType::Down, new ShootCommand(turret, scene, 15, 0.5f));
			dae::InputManager::GetInstance().GetPlayerController(playerId)->AddMapping(inputMappingKeyboard);

			return tankPointer;
		} 

		inline dae::GameObject* CreateProjectile(dae::Scene& scene, dae::GameObject* tank, const glm::vec2 & velocity)
		{
			auto bullet = new dae::GameObject();

			bullet->AddComponent<BounceyPhysicsComponent>(glm::vec2{ 20.f,20.f }, velocity, tank);
			auto renderComponent = bullet->AddComponent<RenderComponentEx>(20.f, 20.f);
			renderComponent->SetTexture("Textures/T_PlayerBullet.png");
			glm::vec2 pos = tank->GetComponent<PhysicsComponent>()->GetPosition();
			bullet->SetPosition(pos.x, pos.y);
			scene.Add(bullet);
			return bullet;
		}
};
}
