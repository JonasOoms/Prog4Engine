#include "HighScoreBuffer.h"
#include "fstream"
#include "json.hpp"

HighScoreBuffer::HighScoreBuffer(const std::string& path)
{
	if (!Deserialize(path))
	{
		// a placeholder score
		Put("FLN", 1);
	}
}

void HighScoreBuffer::Put(const std::string& name, int score)
{
	HighScoreEntry entry{ name, score };
	m_HighScores.emplace_back(entry);

	std::sort(m_HighScores.begin(), m_HighScores.end(),
		[](const HighScoreEntry& a, const HighScoreEntry& b) {
			return b.second < a.second; 
		});


}

size_t HighScoreBuffer::count() const
{
	return m_HighScores.size();
}

const std::vector<HighScoreEntry>& HighScoreBuffer::Get() const
{
	return m_HighScores;
}

bool HighScoreBuffer::Serialize(const std::string& path)
{
	std::ofstream out(path);

	if (!out)
	{
		return false;
	}

	nlohmann::json json = nlohmann::json::array();

	for (const auto& [name, score] : m_HighScores)
	{
		json.push_back({ {"name", name}, {"score", score} });
	}

	out << json.dump(4);

	out.close();
	return true;
}

bool HighScoreBuffer::Deserialize(const std::string& path)
{
	
	std::ifstream in(path);
	if (!in)
	{
		return false;
	}

	nlohmann::json json;
	in >> json;

	m_HighScores.clear();
	for (const auto& entry : json)
	{
		std::string name = entry.at("name").get<std::string>();
		int score = entry.at("score").get<int>();

		HighScoreEntry pair{ name, score };
		m_HighScores.emplace_back(pair);

	}

	return true;

}
