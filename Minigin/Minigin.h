#pragma once
#include <SDL.h>
#include <string>
#include <functional>


namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		const float fixed_time_step{ 0.166667f };
		const long ms_per_frame{ 16 };
		
		const static int windowWidth{ 640 };
		const static int windowHeight{ 480 };

		static SDL_Window* g_window;
		static bool doContinue;
	private:
	};
}