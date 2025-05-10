#pragma once
#include <string>
#include <unordered_map>
namespace Engine
{
	template<typename T>
	class Registry
	{
	public:
		T Get(const  std::string& registryItem);
		void Register(const std::string& registryId, T registryItem);
	private:
		std::unordered_map<std::string, T> m_Registry;
	};

	template<typename T>
	inline T Registry<T>::Get(const std::string& registryId)
	{
		return m_Registry.at(registryId);
	}

	template<typename T>
	inline void Registry<T>::Register(const std::string& registryId, T registryItem)
	{
		m_Registry.insert({ registryId, registryItem });
	};
}