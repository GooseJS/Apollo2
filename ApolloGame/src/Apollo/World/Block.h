#pragma once

#include <string>
#include <vector>

#include <Apollo/render/Texture.h>

namespace Apollo
{
	struct BlockData
	{
		bool renderWithWorld = false;

		bool hasCollision = false;

		// TODO: Read this in from file!
	};

	class BlockManager;
	class Block
	{
	private:
		friend BlockManager;
		int _blockID = -1;
		int _textureID;

		BlockData _data;

		std::string _blockName;
	public:
		Block() {}

		inline int blockID() const { return _blockID; }
		inline int textureID() const { return _textureID; }
		inline std::string name() const { return _blockName; }

		inline BlockData& getData() { return _data; }
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