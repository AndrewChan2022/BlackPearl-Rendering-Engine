#pragma once

#include "BlackPearl/EntryPoint.h"

#include "BlackPearl/Log.h"
#include "BlackPearl/LayerScene/Layer.h"

#include "BlackPearl/Renderer/Shader/Shader.h"
#include "BlackPearl/Renderer/Buffer.h"
#include "BlackPearl/Renderer/Material/CubeMapTexture.h"
#include "BlackPearl/Renderer/Renderer.h"
#include "BlackPearl/Renderer/RenderCommand.h"
#include "BlackPearl/Renderer/MasterRenderer/MasterRenderer.h"
#include "BlackPearl/Renderer/MasterRenderer/VoxelConeTracingRenderer.h"
#include "BlackPearl/Renderer/MasterRenderer/PBRRenderer.h"
#include "BlackPearl/Renderer/MasterRenderer/IBLRenderer.h"
#include "BlackPearl/Renderer/MasterRenderer/BasicRenderer.h"
#include "BlackPearl/Renderer/MasterRenderer/IBLProbesRenderer.h"
#include "BlackPearl/Renderer/MasterRenderer/AnimatedModelRenderer.h"
#include "BlackPearl/Renderer/MasterRenderer/GBufferRenderer.h"
#include "BlackPearl/Renderer/MasterRenderer/VoxelConeTracingDeferredRenderer.h"
#include "BlackPearl/Renderer/MasterRenderer/VoxelConeTracingSVORenderer.h"

#include "BlackPearl/Component/TransformComponent/Transform.h"
#include "BlackPearl/Component/MeshRendererComponent/MeshRenderer.h"
#include "BlackPearl/Component/CameraComponent/Camera.h"
#include "BlackPearl/Component/CameraComponent/PerspectiveCamera.h"
#include "BlackPearl/Component/LightComponent/LightSources.h"
#include "BlackPearl/Component/LightComponent/PointLight.h"
#include "BlackPearl/Component/LightComponent/ParallelLight.h"
#include "BlackPearl/Component/LightComponent/SpotLight.h"
#include "BlackPearl/Component/BaseComponent.h"

#include "BlackPearl/LightProbes/LightProbes.h"
#include "BlackPearl/MainCamera/MainCamera.h"
#include "BlackPearl/Object/Object.h"
#include "BlackPearl/ObjectManager/ObjectManager.h"

#include "BlackPearl/Input.h"
#include "BlackPearl/Timestep/Timestep.h"
#include "BlackPearl/KeyCodes.h"
//#include "BlackPearl/Application.h"

#include "BlackPearl/Scene/Scene.h"
#include "BlackPearl/Entity/Entity.h"
//#include "BlackPearl/Component/ComponentHandle.h"
