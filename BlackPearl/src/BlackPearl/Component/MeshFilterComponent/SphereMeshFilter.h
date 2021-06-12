#pragma once
#include "MeshFilter.h"
namespace BlackPearl {

	class SphereMeshFilter:public MeshFilter
	{
	public:
		SphereMeshFilter(EntityManager * entityManager, Entity::Id id, float radius, unsigned int stackCount,unsigned int sectorCount)
			:MeshFilter(entityManager, id) {
			m_Radius = radius;
			m_StackCount = stackCount;
			m_SectorCount = sectorCount;
			Init();
		}
		void Init();
		float GetRadius() const { return m_Radius; }
		~SphereMeshFilter();
	private:
		float m_Radius;
		unsigned int m_StackCount;//���ȷ�������
		unsigned int m_SectorCount;//γ�ȷ�������

	};

}

