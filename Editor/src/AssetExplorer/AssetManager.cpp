#include "AssetManager.hpp"
#include "Renderer/Renderer.hpp"

namespace engine
{
	AssetNode::AssetNode(bool isFolder, std::weak_ptr<Selectable> asset) : isFolder(isFolder), asset(asset)
	{
		if (isFolder)
			nonOwningIconTexture = AssetManager::getIconTextureForNode(this);

		if (auto lockedAsset = asset.lock())
		{
			if (dynamic_pointer_cast<Texture>(lockedAsset))
			{
				auto texture = dynamic_pointer_cast<Texture>(lockedAsset);
				nonOwningIconTexture = texture;
			}
			else if (dynamic_pointer_cast<Material>(lockedAsset))
			{
				auto material = dynamic_pointer_cast<Material>(lockedAsset);
				RendererSettings settings{};
				owningIconTexture = Renderer::getInstance()->renderTextureOfMaterial(material, &settings);
			}
		}
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
		EventManager::getInstance().subscribe(EventType::SelectableDeleted, this);
		loadIconTextures();
	}

	void AssetManager::addChild(std::shared_ptr<AssetNode> parent, std::shared_ptr<AssetNode> child)
	{
		parent->children.push_back(child);
		child->parent = parent;
		if (child->asset.lock())
		{
			selectableIdToAssetNode[child->asset.lock()->getUUID().id] = child;
		}
	}

	void AssetManager::buildAssetTree()
	{
		rootNode = std::make_shared<AssetNode>(true, std::weak_ptr<Selectable>());
		rootNode->name = "Assets";

		std::shared_ptr<AssetNode> texturesFolderNode = std::make_shared<AssetNode>(true, std::weak_ptr<Selectable>());
		texturesFolderNode->name = "Textures";
		addChild(rootNode, texturesFolderNode);

		for (const auto& [textureId, texture] : game->getTextures())
		{
			std::shared_ptr<AssetNode> textureNode = std::make_shared<AssetNode>(false, texture);
			textureNode->name = texture->getName();
			addChild(texturesFolderNode, textureNode);
		}

		std::shared_ptr<AssetNode> materialsFolderNode = std::make_shared<AssetNode>(true, std::weak_ptr<Selectable>());
		materialsFolderNode->name = "Materials";
		addChild(rootNode, materialsFolderNode);

		for (const auto& [materialId, material] : game->getMaterials())
		{
			std::shared_ptr<AssetNode> materialNode = std::make_shared<AssetNode>(false, material);
			materialNode->name = material->getName();
			addChild(materialsFolderNode, materialNode);
		}

		

	}
	void AssetManager::changeGame(Game* game)
	{
		this->game = game;
		buildAssetTree();
	}
	void AssetManager::loadIconTextures()
	{
		folderIconTexture = std::shared_ptr<Texture>(Texture::create("folder_icon.png", false));
	}

	std::shared_ptr<Texture> AssetManager::getIconTextureForNode(AssetNode* node)
	{
		if (node->isFolder)
			return folderIconTexture;

		return nullptr;
	}
	void AssetManager::onEvent(EventType type, std::string message)
	{
		if (type == EventType::SelectableDeleted)
		{
			// In order to remove the node from the tree, we need to find the parent node and remove the child from it
			AssetNode* node = selectableIdToAssetNode[message].get();
			if (node)
			{
				if (auto lockedParent = node->parent.lock())
				{
					for (auto it = lockedParent->children.begin(); it != lockedParent->children.end(); it++)
					{
						if ((*it)->uuid.id == node->uuid.id)
						{
							lockedParent->children.erase(it);
							break;
						}
					}
				}
				selectableIdToAssetNode.erase(node->asset.lock()->getUUID().id);
			}
		}
	}
}
