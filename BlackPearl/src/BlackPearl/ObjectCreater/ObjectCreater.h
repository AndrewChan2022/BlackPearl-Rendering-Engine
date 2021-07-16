#pragma once
#include "BlackPearl/Entity/Entity.h"
#include "BlackPearl/Object/Object.h"
#include "BlackPearl/Component/LightComponent/Light.h"
#include "BlackPearl/Component/LightComponent/LightSources.h"
#include "BlackPearl/Component/TransformComponent/Transform.h"
#include "BlackPearl/Component/LightProbeComponent/LightProbeComponent.h"

namespace BlackPearl {
	////////////////////////ObjectCreater////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	class ObjectCreater
	{
	public:
		ObjectCreater() {}
		ObjectCreater(EntityManager *entityManager)
			:m_EntityManager(entityManager) {}
		virtual ~ObjectCreater() {
			//if (m_EntityManager != nullptr) {//TODO::���Creater����һ��m_EntityManager�������п��ܱ�ɾ�����
			//	delete m_EntityManager;
			//	m_EntityManager = nullptr;
			//}
		};
		Object* CreateEmpty(std::string name = "");

	protected:
		EntityManager* m_EntityManager;

	};

	////////////////////////Object3DCreater////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	class Object3DCreater :public ObjectCreater{
	public:
		Object3DCreater(EntityManager *entityManager)
		{
			m_EntityManager = entityManager;
		}
		Object* CreateCamera(const std::string name);
		Object* CreateCube(const std::string& shaderPath, const std::string& texturePath,const std::string name);
		Object* CreatePlane(const std::string& shaderPath, const std::string& texturePath, const std::string name);
		Object* CreateSphere(const float radius, const unsigned int stackCount, const unsigned int sectorCount, const std::string& shaderPath, const std::string& texturePath, const std::string name);
		Object* CreateModel(std::string modelPath, std::string shaderPath, const bool isAnimated, const bool addBondingBox, const std::string name);
		Object* CreateSkyBox( const std::vector<std::string>& textureFaces,const std::string& shaderPath, const std::string name);
		Object* CreateLightProbe(ProbeType type, const std::string& shaderPath, const std::string& texturePath, const std::string name);
		Object* CreateGroup(const std::string name);
		Object* CreateBVHNode(const std::vector<Object*>& objs,const std::string name);
		Object* CreateBVHNode(const std::vector<Vertex>& mesh_vertex, const std::string name);


	};

	////////////////////////Object2DCreater////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	class Object2DCreater :public ObjectCreater{
	public:
		Object2DCreater(EntityManager *entityManager)
			:ObjectCreater(entityManager) {}

		Object* CreateQuad(const std::string& shaderPath, const std::string& texturePath, const std::string name);


	};
	////////////////////////LightCreater//////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	class LightCreater :public ObjectCreater {

	public:
		LightCreater(EntityManager *entityManager)
			:ObjectCreater(entityManager) {}

		Object* CreateLight(LightType type, LightSources* lightSources, const std::string name);

	};
	//////////////////////////CameraCreater//////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	//class CameraCreater :public ObjectCreater {

	//public:
	//	CameraCreater(EntityManager *entityManager)
	//		:ObjectCreater(entityManager) {}

	//

	//};
}
