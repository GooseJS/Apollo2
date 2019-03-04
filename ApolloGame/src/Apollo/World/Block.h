#pragma once

#include <string>
#include <vector>

#include <Apollo/render/Texture.h>

#include <Apollo/render/2d/imgui/ApolloImGui.h>

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
		std::vector<std::string> _blockNames;

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

		inline void drawBlockDebugData()
		{
			ImGui::Begin("Block Data");
			if (_blockNames.size() != _registeredBlocks.size())
			{
				_blockNames.clear();
				for (int i = 0; i < _registeredBlocks.size(); i++)
				{
					_blockNames.push_back(_registeredBlocks.at(i).name());
				}
			}

			static int currentlySelectedBlock = 0;
			if (ImGui::BeginCombo("Blocks", _registeredBlocks[currentlySelectedBlock].name().c_str()))
			{
				for (int i = 0; i < _registeredBlocks.size(); i++)
				{
					bool isSelected = (currentlySelectedBlock == i);
					if (ImGui::Selectable(_registeredBlocks[i].name().c_str(), isSelected))
						currentlySelectedBlock = i;
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Checkbox("Collision", &_registeredBlocks[currentlySelectedBlock].getData().hasCollision);

			ImGui::End();
		}
	};
}