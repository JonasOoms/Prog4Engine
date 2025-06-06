
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "TRONGame.h"
#include <memory>

int main(int, char* []) {

	dae::Minigin engine("../Data/");

	std::unique_ptr<TRONGame> game = std::make_unique<TRONGame>();

	engine.Run(std::move(game));
	return 0;
}