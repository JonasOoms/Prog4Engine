#include "SceneManager.h"
#include "Scene.h"
#include "ServiceLocator.h"

void dae::SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
	ServiceLocator::GetPhysicsSystem()->PhysicsUpdate(deltaTime);
}

void dae::SceneManager::FixedUpdate(float fixedTime)
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate(fixedTime);
	}


}

void dae::SceneManager::LateUpdate(float deltaTime)
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}

	ServiceLocator::GetPhysicsSystem()->DebugDraw();

}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
