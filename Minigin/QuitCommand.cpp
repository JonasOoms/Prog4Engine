#include "QuitCommand.h"
#include "Minigin.h"
#include <iostream>

void QuitCommand::Execute()
{
	std::cout << "QuitCommand executed" << std::endl;
	dae::Minigin::doContinue = false;
}
