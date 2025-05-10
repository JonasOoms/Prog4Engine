#pragma once
#include <future>
#include "Level.h"
#include "Scene.h"

class LevelLoader
{
public:
	virtual void Parse(const std::string& filePath, dae::Scene& scene) = 0;
	Level GetLevel() const { return m_Level; }
protected:
	Level m_Level;
};

class JSONLevelLoader : public LevelLoader
{
public:
	JSONLevelLoader() = default;
	//JSONLevelLoader(const std::string& filepath, dae::Scene& scene);

	virtual void Parse(const std::string& filePath, dae::Scene& scene) override;
};
