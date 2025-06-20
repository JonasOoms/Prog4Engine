#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Minigin.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime);
		void FixedUpdate(float fixedTime);
		void LateUpdate(float deltaTime);
		void Render();

		void SelectScene(std::string_view name);
		dae::Scene& GetCurrentScene();

	private:


		friend class Singleton<SceneManager>;
		SceneManager() = default;
		Scene* m_SceneToSwitchTo{};
		void SelectSceneInternal();
		Scene* m_SelectedScene{};
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
