#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>
#include <iostream>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <random>

SDL_Window* dae::Minigin::g_window = nullptr;
float dae::Minigin::m_DeltaTime = 0.0f;
bool dae::Minigin::doContinue = true;

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	std::random_device rd{};
	srand(rd());
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Loading Game...",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}



	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();

 


	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(std::unique_ptr<IGame> game)
{
	m_Game = std::move(game);
	m_Game->Load();

	SDL_SetWindowTitle(g_window, m_Game->GetGameTitle().data());

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	auto last_time = std::chrono::high_resolution_clock::now();
	float lag = 0.0f;
	while (doContinue)
	{
		const auto current_time = std::chrono::high_resolution_clock::now();
		const float delta_time = std::chrono::duration<float>(current_time - last_time).count();
		last_time = current_time;
		lag += delta_time;

		doContinue = input.ProcessInput();

		while (lag >= fixed_time_step)
		{
			sceneManager.FixedUpdate(fixed_time_step);
			lag -= fixed_time_step;
		}

		m_DeltaTime = delta_time;

		sceneManager.Update(delta_time);
		sceneManager.LateUpdate(delta_time);
		renderer.Render();

		const auto sleep_time = current_time + std::chrono::milliseconds(ms_per_frame) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleep_time);

		
	}
}
