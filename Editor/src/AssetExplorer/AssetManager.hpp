#pragma once
#include <memory>
#include <string>
#include <vector>
#include <Engine.hpp>
#include "AssetNode.hpp"

namespace engine
{
	class AssetManager : public EventListener
	{
	public:
		AssetManager(Game* game);

		void buildAssetTree();

		std::shared_ptr<AssetNode> rootNode;

		std::shared_ptr<AssetNode> materialsFolderNode;

		std::shared_ptr<AssetNode> texturesFolderNode;

		std::shared_ptr<AssetNode> scriptsFolderNode;
		
		void changeGame(Game* game);

		static std::shared_ptr<Texture> getIconTextureForNode(AssetNode* node);

		void onEvent(EventType type, std::string message);

		void addChild(std::shared_ptr<AssetNode> parent, std::shared_ptr<AssetNode> child);

		bool isNameInUse(std::shared_ptr<AssetNode> parent, std::string name);

		void addNewScriptNode(const std::string& scriptFileName);
	private:
		void loadIconTextures();

		// In order to be able to delete assets from their selectableId we map it to the assetNode
		std::unordered_map<std::string, std::shared_ptr<AssetNode>> selectableIdToAssetNode{};

		Game* game;

		static inline std::shared_ptr<Texture> folderIconTexture;

		static inline std::shared_ptr<Texture> csharpIconTexture;
	};
}