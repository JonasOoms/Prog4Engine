#pragma once
#include "Component.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "TRONGame.h"
#include "Command.h"
#include "Font.h"

enum class ScoreManagerAction
{
	Up,
	Down,
	Left,
	Right,
	Back,
	Enter
};

class ScoreManagerComponent;
class ScoreGUIState
{
public:
	virtual ~ScoreGUIState() = default;
	virtual std::unique_ptr<ScoreGUIState> Interact(ScoreManagerComponent& scoreManager, ScoreManagerAction action) = 0;
	virtual void Render(ScoreManagerComponent& scoreManager) = 0;
};

class SubmitOrQuitState : public ScoreGUIState
{
public:
	SubmitOrQuitState() = default;
	std::unique_ptr<ScoreGUIState> Interact(ScoreManagerComponent& scoreManager, ScoreManagerAction action) override;

	void Render(ScoreManagerComponent& scoreManager) override;
};

class SubmitScoreState : public ScoreGUIState
{
public:
	SubmitScoreState() = default;
	std::unique_ptr<ScoreGUIState> Interact(ScoreManagerComponent& scoreManager, ScoreManagerAction action) override;
	void Render(ScoreManagerComponent& scoreManager) override;
private:
	uint8_t m_SelectedChar{};
	char m_Name[3] = { 'F', 'L', 'N' };
};

class ScoreManagerComponent : public RenderComponent
{
public:
	ScoreManagerComponent(TRONGame* game);
	virtual void Interact(ScoreManagerAction action);
	virtual void Render() override;

	TRONGame* GetGame();
	dae::Font* GetFont() { return m_ScoreGUIFont.get(); }

private:
	TRONGame* m_Game{};
	std::unique_ptr<ScoreGUIState> m_State;
	std::shared_ptr<dae::Font> m_ScoreGUIFont{};
};

class ScoreManagerCommand : public Command
{
public:
	ScoreManagerCommand(ScoreManagerComponent* scoreManager, ScoreManagerAction action);
private:
	ScoreManagerComponent* m_Binding{};
	ScoreManagerAction m_Action;

	// Inherited via Command
	void Execute() override;
};