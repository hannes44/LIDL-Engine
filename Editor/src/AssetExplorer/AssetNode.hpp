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

		bool isScript = false;

		std::weak_ptr<Selectable> asset;

		std::weak_ptr<AssetNode> parent;

		std::vector<std::shared_ptr<AssetNode>> children;

		std::string name = "";

		UUID uuid{};

		// Returns a vector of this nodes parentage, this nodes parent, the parents parent, etc.
		std::vector<std::weak_ptr<AssetNode>> getEntireParentage();

		// Non owning icon texture, for example if the asset is a texture or a folder, the node will not own the texture
		std::weak_ptr<Texture> nonOwningIconTexture;

		// If the node owns the texture. For example if the asset is a material, the node will own it
		std::shared_ptr<Texture> owningIconTexture = nullptr;

		std::weak_ptr<Texture> getIconTexture() {
			return owningIconTexture ? owningIconTexture : nonOwningIconTexture;
		};
	};
}