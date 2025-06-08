#include "SceneManager.h"
#include "Scene.h"
#include "ServiceLocator.h"

void dae::SceneManager::Update(float deltaTime)
{
	if (!m_SelectedScene) return;
	m_SelectedScene->Update(deltaTime);
	ServiceLocator::GetPhysicsSystem()->PhysicsUpdate(deltaTime);
}

void dae::SceneManager::FixedUpdate(float fixedTime)
{
	if (!m_SelectedScene) return;
	m_SelectedScene->FixedUpdate(fixedTime);
}

void dae::SceneManager::LateUpdate(float deltaTime)
{
	if (m_SelectedScene)
	{
		m_SelectedScene->LateUpdate(deltaTime);
	}
	SelectSceneInternal();
}

void dae::SceneManager::Render()
{

	if (!m_SelectedScene) return;

	
	m_SelectedScene->Render();
	

#ifdef _DEBUG

	
	m_SelectedScene->DebugDraw();
	

#endif // _DEBUG

	ServiceLocator::GetPhysicsSystem()->DebugDraw();
}

void dae::SceneManager::SelectScene(std::string_view name)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [name](const std::shared_ptr<Scene>& scene)
		{
			return (scene->GetName() == name);
		});

	if (it != m_scenes.end())
	{
		if (m_SelectedScene)
		{
			m_SelectedScene->ExitScene();
		}
		m_SceneToSwitchTo = (*it).get();
	}
	else
	{
		std::cout << "Scene " << name << " could not be found!" << std::endl;
	}
}

dae::Scene& dae::SceneManager::GetCurrentScene()
{
	return *m_SelectedScene;
}

void dae::SceneManager::SelectSceneInternal()
{
	if (m_SceneToSwitchTo)
	{
		m_SelectedScene = m_SceneToSwitchTo;
		m_SceneToSwitchTo = nullptr;
		m_SelectedScene->EnterScene();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [name](const std::shared_ptr<Scene>& scene)
		{
			return (scene->GetName() == name);
		});

	if (it != m_scenes.end())
	{
		return *(*it);
	}

	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
