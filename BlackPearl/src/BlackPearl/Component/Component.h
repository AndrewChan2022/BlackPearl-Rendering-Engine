#pragma once
#include <bitset>
#include "BlackPearl/Component/BaseComponent.h"
#include "BlackPearl/Entity/Entity.h"
//#include "BlackPearl/Entity/EntityManager.h"
namespace BlackPearl {
	template<typename C>
	class Component : public BaseComponent 
	{
	public:
		Component(EntityManager* entityManager, Entity::Id id)
		:m_EntityManager(entityManager),m_Id(id){}
		virtual ~Component()=default;

		//friend class EntityManager;		
		static BaseComponent::Family Famliy();
	protected:
		EntityManager* m_EntityManager = nullptr;
		Entity::Id m_Id;
	};

	template<typename C>
	BaseComponent::Family Component<C>::Famliy()
	{
		//static����ֻ��ʼ��һ�Σ�
		//��仰ֻ��ִ��һ�Σ�s_FamliyCounterֻ���ڵ�һ��famliy��ʼ��ʱ��1
		static BaseComponent::Family famliy = BaseComponent::s_FamliyCounter++;
		GE_ASSERT(famliy < MaxComponents, "famliy num exceed MaxComponents!")
			return famliy;
	}
}
