#pragma once
#include <string>
#include <vector>

using HighScoreEntry = std::pair<std::string, int>;

class HighScoreBuffer
{
public:
	HighScoreBuffer(const std::string& path);

	void Put(const std::string& name, int score);
	size_t count() const;
	const std::vector<HighScoreEntry>& Get() const;


	bool Serialize(const std::string& path);
	bool Deserialize(const std::string& path);
private:
	std::vector<HighScoreEntry> m_HighScores; // name - score
};

