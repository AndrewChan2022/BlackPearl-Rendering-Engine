#pragma once
#include <vector>
#include "BlackPearl/Object/Object.h"
#include "BlackPearl/AABB/AABB.h"
#include "BlackPearl/RayTracing/Hitable.h"
#include "BlackPearl/RayTracing/Triangle.h"
namespace BlackPearl {
	class BVHNode : public Component<BVHNode>
	{
	public:
		BVHNode(const std::vector<Object*>& objs);
			
		BVHNode(const std::vector<Vertex>& mesh_vertex);
			
		//BVHNode(const std::vector<Triangle*>& trimesh) 
		//	:Component(Component::Type::BVHNode) {
		//	m_Box = Build(trimesh);
		//}


		AABB GetRootBox() const;
		bool IsLeaf() const;
		void SetLeaf(bool is_leaf);
		Object* GetLeft() const;
		Object* GetRight() const;

	private:
		AABB Build(std::vector<Object*> objs);
		//AABB Build(const std::vector<Triangle*>& triMesh);
		std::vector<Object*> BuildTriMesh(const std::vector<Vertex>& mesh_vertex);

		AABB Build(const std::vector<Hitable*>::const_iterator begin,
			const std::vector<Hitable*>::const_iterator end);

		unsigned int m_BucketsNum = 8;
		Object* m_Left = NULL;
		Object* m_Right = NULL;
		bool m_IsLeaf = false;
		AABB m_Box;
	};
}


