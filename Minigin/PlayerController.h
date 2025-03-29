#pragma once
#include <memory>
#include "SDL.h"
#include "InputMapping.h"

class PlayerController
{
public:
	PlayerController(int id, bool isKeyboard);
	~PlayerController();

	void PollController(const SDL_Event& event = {});
	
	void AddMapping(InputMapping* mapping);

private:

	class impl;
	std::unique_ptr<impl> m_pImpl;

};

