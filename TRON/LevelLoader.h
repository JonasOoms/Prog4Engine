#pragma once
#include <future>
#include "Level.h"
#include "Scene.h"

class LevelLoader
{
public:
	virtual std::unique_ptr<Level> LoadSinglePlayer(const std::string& filePath, dae::Scene& scene) = 0;
	virtual std::unique_ptr<Level> LoadCoOp(const std::string& filePath, dae::Scene& scene) = 0;
	virtual std::unique_ptr<Level> LoadVS(const std::string& filePath, dae::Scene& scene) = 0;
private:
	virtual std::unique_ptr<Level> Parse(const std::string& filePath, dae::Scene& scene) = 0;
};

class JSONLevelLoader : public LevelLoader
{
public:
	JSONLevelLoader() = default;
	//JSONLevelLoader(const std::string& filepath, dae::Scene& scene);
	virtual std::unique_ptr<Level> LoadSinglePlayer(const std::string& filePath, dae::Scene& scene) override;
	virtual std::unique_ptr<Level> LoadCoOp(const std::string& filePath, dae::Scene& scene) override;
	virtual std::unique_ptr<Level> LoadVS(const std::string& filePath, dae::Scene& scene) override;
private:
	virtual std::unique_ptr<Level> Parse(const std::string& filePath, dae::Scene& scene) override;
};
