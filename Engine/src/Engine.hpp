#pragma once

// Single include for game files

// Core
#include "Core/Game.hpp"
#include "Core/GameObject.hpp"
#include "Core/Logger.hpp"
#include "Core/Material.hpp"
#include "Core/Texture.hpp"
#include "Core/Transform.hpp"
#include "Core/Uuid.hpp"
#include "Core/Window.hpp"
#include "Core/Debug.hpp"

// Rendering
#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Buffer.hpp"
#include "Renderer/RendererSettings.hpp"

// Components
#include "Components/Component.hpp"
#include "Components/MeshComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/BoxColliderComponent.hpp"
#include "Components/SphereColliderComponent.hpp"
#include "Components/PointLightComponent.hpp"
#include "Components/CameraComponent.hpp"

//Events
#include "Events/EventManager.hpp"

//Input
#include "Input/InputEvent.hpp"
#include "Input/InputFrameWork.hpp"
#include "Input/InputListener.hpp"
#include "Input/InputFrameWork.hpp"