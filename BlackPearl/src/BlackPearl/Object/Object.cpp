#include "pch.h"
#include "Object.h"
#include "BlackPearl/Component/CameraComponent/PerspectiveCamera.h"
#include <utility>
namespace BlackPearl {


	
	Configuration::ComponentMask Object::GetComponentMask() const
	{
		return m_ComponentMask;
	}



	void Object::Destroy()
	{
		//for (auto& pair : m_Components) { //TODO::��һ��Ҫ��Ҫ������ָ��
		//	auto component = pair.second;
		//	component.reset(nullptr);
		//	
		//}
		m_ComponentMask.reset();
		m_EntityManager->DestroyEntity(m_Id);	
		m_Components.clear();
	}

}
