#pragma once
#include <SDL.h>
#include <string>
#include <functional>
#include <memory>
#include "IGame.h"

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(std::unique_ptr<IGame> game);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		static float GetDeltaTime()
		{
			return m_DeltaTime;
		}

		const float fixed_time_step{ 0.166667f };
		const long ms_per_frame{ 16 };
		
		const static int windowWidth{ 650 };
		const static int windowHeight{ 650 };

		static SDL_Window* g_window;
		static bool doContinue;
	private:
		static float m_DeltaTime;

		std::unique_ptr<IGame> m_Game;

	};
}