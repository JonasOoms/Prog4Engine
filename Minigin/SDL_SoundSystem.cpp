#include "SDL_SoundSystem.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <cassert>
class SDL_SoundSystem::impl
{
public:
	impl()
	{

		if (SDL_Init(SDL_INIT_AUDIO) < 0)
		{
			std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		}

		m_AudioThread = std::jthread([this](std::stop_token token) {
			this->Consume(token);
			});
	}

	~impl()
	{
		m_AudioThread.request_stop();
		cv.notify_one();
		m_AudioThread.join();
		Mix_CloseAudio();
	}

	void Mute(bool isMuted)
	{
		std::lock_guard lock(m_AudioThreadMutex);
		m_isMuted = isMuted;
		if (isMuted)
		{
			Mix_Volume(-1, 0);
		}
		else
		{
			Mix_Volume(-1, MIX_MAX_VOLUME);
		}
	}

	bool GetIsMuted()
	{
		std::lock_guard lock(m_AudioThreadMutex);
		return m_isMuted;
	}

	/// <summary>
	/// Registers an audio into the internal audio system. Returns the integer
	/// audio id that can be used to play the audio.
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	sound_id RegisterAudio(const std::string& filePath)
	{
		m_AudioClips.emplace_back(std::make_unique<AudioClip>(filePath));
		return static_cast<sound_id>(m_AudioClips.size() - 1);
	}

	/// <summary>
	/// Pushes a sound request with the sound id and volume to the sound queue. The audio system takes care of playing the sound.
	/// </summary>
	/// <param name="id"></param>
	/// <param name="volume"></param>
	void PushSoundRequest(const sound_id id, const float volume)
	{
		if (id >= m_AudioClips.size())
		{
			std::cerr << "Invalid sound ID: " << id << std::endl;
			return;
		}
		m_SoundQueue.Push({ id, volume });
		cv.notify_one();
	}

private:

	struct SoundRequest
	{
		sound_id id;
		float volume;
	};

	class AudioClip
	{
	public:
		AudioClip(const std::string& path):
			m_Path(path),
			m_isLoaded(false),
			m_pChunk(nullptr),
			m_Volume(128)
		{
		}
		
		~AudioClip()
		{
			if (m_isLoaded)
			{
				Mix_FreeChunk(m_pChunk);
				m_pChunk = nullptr;
			}
		}

		bool isLoaded() const
		{
			return m_isLoaded;
		}	

		void Load()
		{
			std::lock_guard lock(m_Mutex);
			if (!m_isLoaded)
			{
				m_pChunk = Mix_LoadWAV(m_Path.c_str());
				if (m_pChunk == nullptr)
				{
					std::cerr << "Failed to load sound file: " << m_Path << " Error: " << Mix_GetError() << std::endl;
					assert(m_pChunk == nullptr);
				}
				else
				{
					m_isLoaded = true;
				}
			}
		}

		void SetVolume(const float volume)
		{
			std::lock_guard lock(m_Mutex);
			if (m_isLoaded)
			{
				Mix_VolumeChunk(m_pChunk, static_cast<int>(std::min(volume, static_cast<float>(MIX_MAX_VOLUME))));
			}
		}
	
		void Play()
		{
			std::lock_guard lock(m_Mutex);
			if (m_isLoaded)
			{
				Mix_PlayChannel(-1, m_pChunk, 0);
			}
		}

	private:
		bool m_isLoaded{};
		float m_Volume;
		Mix_Chunk* m_pChunk;
		std::mutex m_Mutex;
		std::string m_Path;
	};

	class SoundQueue
	{
	public:
		void Push(const SoundRequest& request)
		{
			std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
			m_SoundQueue.push(request);
		}
		bool Pop(SoundRequest& request)
		{
			std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
			if (m_SoundQueue.empty())
			{
				return false;
			}
			request = std::move(m_SoundQueue.front());
			m_SoundQueue.pop();
			return true;
		}

		bool IsEmpty() 
		{
			std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
			return m_SoundQueue.empty();
		}

		size_t Size()
		{
			std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
			return m_SoundQueue.size();
		}

	private:
		std::queue<SoundRequest> m_SoundQueue;
		std::mutex m_SoundQueueMutex;
	};

	void Consume(std::stop_token token)
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(m_AudioThreadMutex);
			cv.wait(lock, [&] {return !m_SoundQueue.IsEmpty() || token.stop_requested(); });

			if (token.stop_requested())
			{
				break;
			}

			SoundRequest request;
			m_SoundQueue.Pop(request);
			auto audioclip = m_AudioClips[request.id].get();
			if (!audioclip->isLoaded())
			{
				audioclip->Load();
			}

			if (!m_isMuted)
			{
				audioclip->SetVolume(request.volume);
			}
			m_LastPlayingVolume = request.volume;
			audioclip->Play();
			
		}
	}

	float m_LastPlayingVolume{};
	bool m_isMuted{ false };
	std::jthread m_AudioThread;
	bool m_isStopRequested = false;
	std::mutex m_AudioThreadMutex;
	std::condition_variable cv;
	std::vector<std::unique_ptr<AudioClip>> m_AudioClips;
	SoundQueue m_SoundQueue;

};

SDL_SoundSystem::~SDL_SoundSystem() = default;

SDL_SoundSystem::SDL_SoundSystem():
	m_pImpl(std::make_unique<impl>())
{
}

sound_id SDL_SoundSystem::RegisterAudio(const std::string& filePath)
{
	return m_pImpl->RegisterAudio(filePath);
}

void SDL_SoundSystem::Mute(bool isMuted)
{
	m_pImpl->Mute(isMuted);
}

bool SDL_SoundSystem::GetIsMuted()
{
	return m_pImpl->GetIsMuted();
}

void SDL_SoundSystem::Play(const sound_id id, const float volume)
{
	m_pImpl->PushSoundRequest(id, volume);
}
