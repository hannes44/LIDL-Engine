#pragma once
#include <memory>
#include <string>
#include <vector>
#include <Engine.hpp>

namespace engine
{
	class AssetNode
	{
	public:
		AssetNode(bool isFolder, std::weak_ptr<Selectable> asset);

		bool isFolder = false;

		std::weak_ptr<Selectable> asset;

		std::weak_ptr<AssetNode> parent;

		std::vector<std::shared_ptr<AssetNode>> children;

		std::string name = "";

		UUID uuid{};

		// Returns a vector of this nodes parentage, this nodes parent, the parents parent, etc.
		std::vector<std::weak_ptr<AssetNode>> getEntireParentage();

		std::shared_ptr<Texture> iconTexture;

		
	};
}