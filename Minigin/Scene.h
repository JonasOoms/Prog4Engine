#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(dae::GameObject* object);
		void Add(std::unique_ptr<GameObject>&& gameObject);
		void Remove(dae::GameObject* object);
		void RemoveAll();

		void ExitScene();
		void EnterScene();
		void Update(float deltaTime);
		void FixedUpdate(float fixedTime);
		void LateUpdate(float deltaTime);
		void Render() const;
		void DebugDraw() const;

		void DestroyCheck();

		std::string_view GetName() const {return m_name;}

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		void FlushPendingAdd();

		std::string m_name;
		std::vector < std::unique_ptr<GameObject>> m_objects{};
		std::vector<std::unique_ptr<GameObject>> m_ToAdd{};

		static unsigned int m_idCounter; 
	};

}
