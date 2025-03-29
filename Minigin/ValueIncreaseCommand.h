#pragma once 
#include "ValueComponent.h"
#include "Command.h"

template<typename T>
class ValueIncreaseCommand : public Command
{
public:
	ValueIncreaseCommand(T increaseAmount, ValueComponent<T>* valueComponent);

	void Execute() override;
private:
	T m_increaseAmount;
	ValueComponent<T>* m_ValueComponent;
};

template<typename T>
inline ValueIncreaseCommand<T>::ValueIncreaseCommand(T increaseAmount, ValueComponent<T>* ValueComponent) :
	m_ValueComponent{ ValueComponent },
	m_increaseAmount{ increaseAmount }
{
}

template <typename T>
inline void ValueIncreaseCommand<T>::Execute()
{
	m_ValueComponent->Increment(m_increaseAmount);
}
