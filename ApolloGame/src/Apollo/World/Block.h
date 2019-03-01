#pragma once

#include <string>
#include <vector>

#include <Apollo/render/Texture.h>

namespace Apollo
{
	class BlockManager;
	class Block
	{
	private:
		friend BlockManager;
		int _blockID = -1;
		int _textureID;

		std::string _blockName;
	public:
		Block() {}

		int blockID() const { return _blockID; }
		int textureID() const { return _textureID; }
		std::string name() const { return _blockName; }
	};

	class BlockManager
	{
	private:
		BlockManager() {}
		~BlockManager() = default;

		std::vector<Block> _registeredBlocks;

		int getLastBlockID() { return _registeredBlocks.size(); }
	public:
		static BlockManager& getInstance()
		{
			static BlockManager instance;
			return instance;
		}

		inline int addBlock(std::string name, int textureID) // TODO: This is eventually gonna read from probably JSON files
		{
			Block block;
			block._blockID = getLastBlockID();
			block._textureID = textureID;
			block._blockName = name;
			_registeredBlocks.push_back(block);
			return getLastBlockID() - 1;
		}

		inline Block& getBlock(int blockID)
		{
			return _registeredBlocks.at(blockID);
		}
	};
}