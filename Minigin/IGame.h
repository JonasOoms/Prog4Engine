#pragma once
#include <string_view>

class IGame
{
public:
	virtual ~IGame() = default;
	virtual void Load() = 0;
	virtual std::string_view GetGameTitle() = 0;
};
