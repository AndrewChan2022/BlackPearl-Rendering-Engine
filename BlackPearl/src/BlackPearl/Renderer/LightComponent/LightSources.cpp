#include "pch.h"
#include "LightSources.h"
namespace BlackPearl {

	void LightSources::AddLight(const std::shared_ptr<Light>& light)
	{
		m_LightSources.push_back(light);
		if (light->GetType() == LightType::PointLight)
			m_PointLightNums++;
	}
}