#include "AssetManager.hpp"

namespace engine
{
	AssetNode::AssetNode(bool isFolder, std::weak_ptr<Selectable> asset) : isFolder(isFolder), asset(asset)
	{
		if (isFolder)
			iconTexture = AssetManager::getIconTextureForNode(this);
	}
	void AssetNode::addChild(std::shared_ptr<AssetNode> parent, std::shared_ptr<AssetNode> child)
	{
		parent->children.push_back(child);
		child->parent = parent;
	}
	std::vector<std::weak_ptr<AssetNode>> AssetNode::getEntireParentage()
	{
		std::vector<std::weak_ptr<AssetNode>> parents;
		std::weak_ptr<AssetNode> currentParent = parent;
		while (auto lockedCurrentParent = currentParent.lock())
		{
			parents.push_back(lockedCurrentParent);
			currentParent = lockedCurrentParent->parent;
		}
		return parents;
	}
	AssetManager::AssetManager(Game* game) : game(game)
	{
		loadIconTextures();
	}

	void AssetManager::buildAssetTree()
	{
		rootNode = std::make_shared<AssetNode>(true, std::weak_ptr<Selectable>());
		rootNode->name = "Assets";

		std::shared_ptr<AssetNode> texturesFolderNode = std::make_shared<AssetNode>(true, std::weak_ptr<Selectable>());
		texturesFolderNode->name = "Textures";
		AssetNode::addChild(rootNode, texturesFolderNode);

		for (const auto& [textureId, texture] : game->textures)
		{
			std::shared_ptr<AssetNode> textureNode = std::make_shared<AssetNode>(false, texture);
			textureNode->name = texture->getName();
			AssetNode::addChild(texturesFolderNode, textureNode);
		}

		std::shared_ptr<AssetNode> materialsFolderNode = std::make_shared<AssetNode>(true, std::weak_ptr<Selectable>());
		materialsFolderNode->name = "Materials";
		AssetNode::addChild(rootNode, materialsFolderNode);

		

	}
	void AssetManager::changeGame(Game* game)
	{
		this->game = game;
		buildAssetTree();
	}
	void AssetManager::loadIconTextures()
	{
		folderIconTexture = std::shared_ptr<Texture>(Texture::create("folder_icon.png"));
	}
	std::shared_ptr<Texture> AssetManager::getIconTextureForNode(AssetNode* node)
	{
		if (node->isFolder)
			return folderIconTexture;

		return nullptr;
	}
}
