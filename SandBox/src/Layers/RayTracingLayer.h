#pragma once
#pragma once
#include <BlackPearl.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imfilebrowser.h"
#include "BlackPearl/Math/Math.h"
#include <glm/gtc/type_ptr.hpp>
class RayTracingLayer :public BlackPearl::Layer {
public:

	RayTracingLayer(const std::string& name)
		: Layer(name)
	{


		m_BasicRenderer = DBG_NEW BlackPearl::BasicRenderer();
		m_RayTracingRenderer = DBG_NEW BlackPearl::RayTracingRenderer();
		m_SceneBuilder = DBG_NEW BlackPearl::SceneBuilder();

		m_QuadObj = CreateQuad();
		m_RayTracingRenderer->Init(m_QuadObj);

		//Group
#if 0
		{
			m_Sphere1 = CreateSphere(1.0f, 64, 64);
			m_Sphere2 = CreateSphere(1.0f, 64, 64);
			m_Sphere3 = CreateSphere(1.0f, 64, 64);
			m_Sphere4 = CreateSphere(1.0f, 64, 64);
			m_Sphere5 = CreateSphere(8.0f, 64, 64);

			m_Sphere1->GetComponent<BlackPearl::MeshRenderer>()->GetMeshes()[0].GetMaterial()->SetMaterialColorSpecularColor({ 0.6,0.7,BlackPearl::Math::Rand_F() });
			m_Sphere2->GetComponent<BlackPearl::MeshRenderer>()->GetMeshes()[0].GetMaterial()->SetMaterialColorSpecularColor({ 0.9,0.9,0 });
			m_Sphere3->GetComponent<BlackPearl::MeshRenderer>()->GetMeshes()[0].GetMaterial()->SetMaterialColorSpecularColor({ BlackPearl::Math::Rand_F(),BlackPearl::Math::Rand_F(),BlackPearl::Math::Rand_F() });
			m_Sphere4->GetComponent<BlackPearl::MeshRenderer>()->GetMeshes()[0].GetMaterial()->SetMaterialColorSpecularColor({ 0.9,0,0.26 });
			m_Sphere5->GetComponent<BlackPearl::MeshRenderer>()->GetMeshes()[0].GetMaterial()->SetMaterialColorSpecularColor({ BlackPearl::Math::Rand_F(),BlackPearl::Math::Rand_F(),0.8 });

			m_Sphere1->GetComponent<BlackPearl::Transform>()->SetInitPosition({ 1.0,0.0,0.0 });
			m_Sphere2->GetComponent<BlackPearl::Transform>()->SetInitPosition({ -1.0,0.0,0.0 });
			m_Sphere3->GetComponent<BlackPearl::Transform>()->SetInitPosition({ 3.0,0.0,0.0 });
			m_Sphere4->GetComponent<BlackPearl::Transform>()->SetInitPosition({ -3.0,0.0,0.0 });
			m_Sphere5->GetComponent<BlackPearl::Transform>()->SetInitPosition({ 0.0,-9.0,0.0 });

			group_obj = BlackPearl::Layer::CreateGroup();
			m_Group = DBG_NEW BlackPearl::Group(group_obj);

			m_Group->PushBack(m_Sphere1);
			m_Group->PushBack(m_Sphere2);
			m_Group->PushBack(m_Sphere3);
			m_Group->PushBack(m_Sphere4);
			m_Group->PushBack(m_Sphere5);

			m_SceneBuilder->CreateSceneData(m_Group);
			m_RayTracingRenderer->InitScene(m_SceneBuilder->GetScene());
		}
#endif

		//BVHNode
		{
			m_Bunny = CreateModel("assets/models/bunny/bunny.obj", "assets/shaders/IronMan.glsl", false, "Bunny");
			m_Bunny->GetComponent<BlackPearl::Transform>()->SetInitScale(glm::vec3(0.5));
			m_Bunny->GetComponent<BlackPearl::Transform>()->SetInitPosition({ 0.6f,0.0f,3.0f });
			m_Bunny->GetComponent<BlackPearl::Transform>()->SetInitRotation({ 0.0f,-30.0f,0.0f });
			m_Bunny->GetComponent<BlackPearl::MeshRenderer>()->SetIsBackGroundObjects(true);

			m_BVHNode = BlackPearl::g_objectManager->CreateBVHNode(m_Bunny->GetComponent<BlackPearl::MeshRenderer>()->GetModel()->GetMeshVertex());
			m_SceneBuilder->CreateSceneData(m_BVHNode);
			m_RayTracingRenderer->InitScene(m_SceneBuilder->GetScene());
		}
		m_MainCamera->SetFov(90.0f);
		m_MainCamera->SetPosition(glm::vec3(0, 0, 5));//glm::vec3(0,1.387f,22.012f)
		m_CameraPosition = m_MainCamera->GetPosition();
		BlackPearl::Renderer::Init();

		/***************************************** Scene ********************************************************/
		m_SkyBoxObj = CreateSkyBox(
			{ "assets/skybox/skybox/right.jpg",
			 "assets/skybox/skybox/left.jpg",
			 "assets/skybox/skybox/top.jpg",
			 "assets/skybox/skybox/bottom.jpg",
			 "assets/skybox/skybox/front.jpg",
			 "assets/skybox/skybox/back.jpg",
			});
		m_skybox = m_SkyBoxObj;
		/*******************************************************************************************************/
		/*******************************************************************************************************/





	}

	virtual ~RayTracingLayer() {

		DestroyObjects();

	}
	void OnUpdate(BlackPearl::Timestep ts) override {


		InputCheck(ts);
		milliseconds currentTimeMs = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		double runtimeSecond = (currentTimeMs.count() - m_StartTimeMs.count()) / 1000.0f;
	



		BlackPearl::RenderCommand::SetClearColor(m_BackgroundColor);
		BlackPearl::Renderer::BeginScene(*(m_MainCamera->GetObj()->GetComponent<BlackPearl::PerspectiveCamera>()), *GetLightSources());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, BlackPearl::Configuration::WindowWidth, BlackPearl::Configuration::WindowHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (BlackPearl::Input::IsKeyPressed(BP_KEY_Y)) {

			m_skybox = m_SkyBoxObj;
			GE_CORE_INFO("m_skybox = m_SkyBoxObj");
		}
		if (BlackPearl::Input::IsKeyPressed(BP_KEY_N)) {

			m_skybox = nullptr;
			GE_CORE_INFO("m_skybox = nullptr");

		}

		//m_RayTracingRenderer->RenderSpheres(m_MainCamera);
	//	m_RayTracingRenderer->Render();
	//	m_RayTracingRenderer->RenderMaterialSpheres(m_MainCamera);
		//m_RayTracingRenderer->RenderGroup(m_MainCamera, m_Group->GetRoot());
		m_RayTracingRenderer->RenderBVHNode(m_MainCamera, m_BVHNode);

	

	}

	void OnAttach() override {


	}

private:

	std::vector<BlackPearl::Object*> m_LightObjs;
	BlackPearl::Object* m_QuadObj;
	BlackPearl::Group* m_Group;
	BlackPearl::Object* m_Sphere1;
	BlackPearl::Object* m_Sphere2;
	BlackPearl::Object* m_Sphere3;
	BlackPearl::Object* m_Sphere4;
	BlackPearl::Object* m_Sphere5;

	BlackPearl::Object* m_Bunny;
	BlackPearl::Object* m_BVHNode;

	BlackPearl::Object* m_SkyBoxObj;
	BlackPearl::Object* m_skybox = nullptr;
	BlackPearl::Object* group_obj;
	glm::vec4 m_BackgroundColor = { 0.0f,0.0f,0.0f,0.0f };


	BlackPearl::RayTracingRenderer* m_RayTracingRenderer;
	BlackPearl::BasicRenderer* m_BasicRenderer;
	BlackPearl::SceneBuilder* m_SceneBuilder;

};
