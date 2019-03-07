#pragma once

#include <string>
#include <filesystem>

#include "Apollo/Core.h"

namespace Apollo
{
	class APOLLO_API ResourceManager
	{
	private:
		ResourceManager() {}
		~ResourceManager() {}

		std::string _dataDir;
	public: // TODO: This works for now, but it's going to have to get completely overhauled into a system packed into our own files.
		inline static ResourceManager& getInstance()
		{
			static ResourceManager instance;
			return instance;
		}

		inline void setDataDir(const std::string& newDataDir) { _dataDir = newDataDir; }
		inline std::string getDataDir() const { return _dataDir; }

		bool doesFileExist(const std::string& fileName) const { return std::filesystem::exists(_dataDir + fileName); }
		std::string getDataLocation(const std::string& fileName) const { return _dataDir + fileName; }
	};
}