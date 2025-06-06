#pragma once
#include "Command.h"
#include <vector>
#include <memory>
#include "SDL.h"

enum class TriggerType
{
	Pressed,
	Down,
	Released

};

struct InputBinding
{
		unsigned int button{};
		TriggerType triggerType{};
		std::unique_ptr<Command> command;
};

template<typename T>
concept CommandType = std::is_base_of<Command, T>::value;

class InputMapping
{
public:
	InputMapping();
	~InputMapping();

	void PollInputMapping(unsigned short buttons, unsigned short buttonsPressedThisFrame, unsigned short buttonsReleasedThisFrame);
	void PollInputMapping(const SDL_Event& event);

	template<CommandType T>
	void AddInputBinding(unsigned int button, TriggerType triggertype, std::unique_ptr<T> command);

private:
	std::vector<InputBinding> m_InputBindings;
};


template<CommandType T>
inline void InputMapping::AddInputBinding(unsigned int button, TriggerType triggertype, std::unique_ptr<T> command)
{ 
	m_InputBindings.emplace_back(button, triggertype, std::move(command));
};
