#pragma once
#include "Component.h"
#include "Level.h"
#include "Observer.h"
#include <memory>
#include "TRONGame.h"
#include "TextRenderComponent.h"

class LevelManagerComponent final : public Component, public Observer
{
public:
	
	LevelManagerComponent(TRONGame* game, TextRenderComponent* scoreCounter, TextRenderComponent* lifeCounter ,TextRenderComponent* highScoreCounter);

	virtual void BeginPlay() override;
	virtual void EndPlay() override;

	virtual void Update(float deltaTime) override;

	EventDispatcher* GetEventDispatcher() { return m_EventDispatcher.get(); }
private:

	TRONGame* m_Game{};
	TextRenderComponent* m_ScoreCounter{};
	TextRenderComponent* m_LifeCounter{};
	TextRenderComponent* m_HighScoreCounter{};

	int m_Score{};
	int m_Lives{3};
	

	void LoadLevel(const std::string& filePath);
	void ClearLevel();

	bool m_ExitLevel{};
	bool m_LoadNextLevel{};
	int m_SelectedLevel{0};
	std::unique_ptr<Level> m_Level;
	std::unique_ptr<EventDispatcher> m_EventDispatcher;
	size_t m_EnemiesLeft{ 0 };
	// Inherited via Observer
	void Notify(const Event& event, EventDispatcher* subject) override;
};

