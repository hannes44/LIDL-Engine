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

		void changeGame(Game* game);

		void onEvent(EventType type, std::string message);

		void addChild(std::shared_ptr<AssetNode> parent, std::shared_ptr<AssetNode> child);

		bool isNameInUse(std::shared_ptr<AssetNode> parent, std::string name);

		void deleteAssetNode(std::shared_ptr<AssetNode> node);

		void addNewScriptNode(const std::string& scriptFileName);

		static std::shared_ptr<Texture> getIconTextureForNode(AssetNode* node);

		std::shared_ptr<AssetNode> rootNode;
	private:
		void loadIconTextures();

		Game* game;

		// In order to be able to delete assets from their selectableId we map it to the assetNode
		std::unordered_map<std::string, std::shared_ptr<AssetNode>> selectableIdToAssetNode{};

		static inline std::shared_ptr<Texture> folderIconTexture;
		static inline std::shared_ptr<Texture> csharpIconTexture;

		std::shared_ptr<AssetNode> materialsFolderNode;
		std::shared_ptr<AssetNode> texturesFolderNode;
		std::shared_ptr<AssetNode> scriptsFolderNode;
	};
}