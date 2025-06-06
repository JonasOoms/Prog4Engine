#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(dae::GameObject* object)
{
	m_objects.emplace_back(std::unique_ptr<GameObject>{object});
}

void dae::Scene::Add(std::unique_ptr<GameObject>&& gameObject)
{
	m_objects.emplace_back(std::move(gameObject));
}

void Scene::Remove(dae::GameObject* object)
{
	m_objects.erase(
		std::remove_if(m_objects.begin(), m_objects.end(),
			[object](const std::unique_ptr<GameObject>& ptr)
			{
				return ptr.get() == object;
			}),
		m_objects.end()
	);
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void dae::Scene::ExitScene()
{
	for (auto& object : m_objects)
	{
		object->EndPlay();
	}
}

void dae::Scene::EnterScene()
{
	for (auto& object : m_objects)
	{
		object->BeginPlay();
	}
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}
}

void dae::Scene::FixedUpdate(float fixedTime)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(fixedTime);
	}
}

void dae::Scene::LateUpdate(float deltaTime)
{
	for (auto& object : m_objects)
	{
		object->LateUpdate(deltaTime);
	}

	std::erase_if(m_objects, [](const auto& object)
		{
			return object->GetDeleteFlag();
		});
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void dae::Scene::DebugDraw() const
{
	for (const auto& object : m_objects)
	{
		object->DebugDraw();
	}
}

