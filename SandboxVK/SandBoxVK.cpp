#include <BlackPearl.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "BlackPearl/Application.h"
#include "BlackPearl/RHI/DynamicRHI.h"

#include "BlackPearl/RHI/DynamicRHI.h"
#include "VkTestLayer.h"
class SandBoxVK :public BlackPearl::Application {

public:

	SandBoxVK(HINSTANCE hInstance, int nShowCmd, BlackPearl::DynamicRHI::Type rhiType, const std::string& renderer)
		:Application(hInstance, nShowCmd, rhiType, renderer) {

		BlackPearl::Layer* layer = NULL;
		const std::string layer_name = renderer + "Layer";

		if (renderer == "VkTest") {
			layer = DBG_NEW VkTestLayer(layer_name);
		}
		
		GetLayerManager()->PushLayer(layer);
	}
	virtual ~SandBoxVK() = default;

};

BlackPearl::Application* BlackPearl::CreateApplication(HINSTANCE hInstance, int nShowCmd) {
	return DBG_NEW SandBoxVK(hInstance, nShowCmd, BlackPearl::DynamicRHI::Type::Vulkan, "VkTest");

}

