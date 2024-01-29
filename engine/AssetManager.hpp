#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Uuid.hpp"
#include "Game.hpp"
#include "AssetNode.hpp"

namespace engine
{
	class AssetManager
	{
	public:
		AssetManager(Game* game);

		void buildAssetTree();

		std::shared_ptr<AssetNode> rootNode;
	
	private:
		std::unordered_map<std::string, std::weak_ptr<AssetNode>> assetNodes;

		Game* game;
	};
}