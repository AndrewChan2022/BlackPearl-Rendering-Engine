#pragma once

#include <BlackPeral.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imfilebrowser.h"
#include <thread>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
using namespace std::chrono;

class IBLProbesRenderingLayer :public BlackPearl::Layer {
public:

	IBLProbesRenderingLayer(const std::string& name, BlackPearl::ObjectManager* objectManager)
		: Layer(name, objectManager)
	{

		m_StartTimeMs = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		m_MainCamera->SetPosition(glm::vec3(-2.0f, 0.0f, 14.0f));

		
		///*ProbesCamera is used to render probes'environmentMap*/
		//m_ProbesCamera = CreateCamera();
		//m_ProbesCamera->GetComponent<BlackPearl::PerspectiveCamera>()->SetFov(90.0f);
		///* make sure aspectRadio = 1 */
		//m_ProbesCamera->GetComponent<BlackPearl::PerspectiveCamera>()->SetWidth(512);
		//m_ProbesCamera->GetComponent<BlackPearl::PerspectiveCamera>()->SetHeight(512);

		

		//Shader
		m_BackGroundShader.reset(DBG_NEW BlackPearl::Shader("assets/shaders/ibl/background.glsl"));
		//Scene
		//m_SphereObj = CreateSphere(1.5, 64, 64);
		m_SphereObjIron = CreateSphere(0.5, 64, 64);
		m_SphereObjRust = CreateSphere(0.5, 64, 64);
		m_SphereObjStone = CreateSphere(0.5, 64, 64);
		m_SphereObjPlastic = CreateSphere(0.5, 64, 64);
		m_DebugQuadShader.reset(DBG_NEW BlackPearl::Shader("assets/shaders/QuadDebug.glsl"));

		/* probe's CubeObj and quad for BrdfLUTMap */
		m_BrdfLUTQuadObj = CreateQuad("assets/shaders/ibl/brdf.glsl","");
		m_SHImageQuadObj = CreateQuad("assets/shaders/lightProbes/SH.glsl", "");
		m_DebugQuad = CreateQuad("assets/shaders/QuadDebug.glsl","");

		/* create probes */
		unsigned int xlen = 1, ylen = 3,zlen = 1,space = 3;
		for (unsigned int x = 0; x < xlen; x++)
		{
			for (unsigned int y = 0; y < ylen; y++)
			{
				for (unsigned int  z = 0; z < zlen; z++)
				{
					BlackPearl::LightProbe* probe = CreateLightProbe();
					int xx = (x - xlen / 2) * space, yy = (y - ylen / 2) * space, zz = (z - zlen / 2) * space;
					
					probe->SetPosition({xx,yy,zz});
					m_LightProbes.push_back(probe);
					//delete probe;

				}
				
			}

		}




		

		BlackPearl::Renderer::Init();
	//	glDepthFunc(GL_LEQUAL);
		/*Renderer*/
		m_BasicRenderer = DBG_NEW BlackPearl::BasicRenderer();
		m_IBLProbesRenderer = DBG_NEW BlackPearl::IBLProbesRenderer();
		m_AnimatedModelRenderer = DBG_NEW BlackPearl::AnimatedModelRenderer();
		/*create skybox */
		/*notice: draw skybox before anything else!*/
		m_SkyBoxObj1 = CreateSkyBox(
			{ "assets/skybox/skybox/right.jpg",
			 "assets/skybox/skybox/left.jpg",
			 "assets/skybox/skybox/top.jpg",
			 "assets/skybox/skybox/bottom.jpg",
			 "assets/skybox/skybox/front.jpg",
			 "assets/skybox/skybox/back.jpg",
			});
		  
		/*model animation*/
		m_AnimatedModel = CreateModel("assets/models-animation/boblampclean.md5mesh", "assets/shaders/animatedModel/animatedModel.glsl",true);
		m_AnimatedModel->GetComponent<BlackPearl::Transform>()->SetScale({ 0.05f,0.05f,0.05f });
		m_AnimatedModel->GetComponent<BlackPearl::Transform>()->SetRotation({ 90.0f,180.0f,180.0f });
		m_AnimatedModel->GetComponent<BlackPearl::Transform>()->SetPosition({ 0.0f,-1.6f,0.0f });

	//	m_AnimatedModel->GetComponent<BlackPearl::MeshRenderer>()
		//m_BackGroundObjsList.push_back(m_SkyBoxObj1);
		/*create model*/
		/*BlackPearl::Object *deer=  CreateModel("assets/models/deer/Deer.obj", "assets/shaders/IronMan.glsl");
		deer->GetComponent<BlackPearl::Transform>()->SetScale(glm::vec3(0.005));
		deer->GetComponent<BlackPearl::Transform>()->SetPosition({0.0f,-1.5f,1.0f});
		m_BackGroundObjsList.push_back(deer);*/

		BlackPearl::Object* ironMan = CreateModel("assets/models/IronMan/IronMan.obj", "assets/shaders/IronMan.glsl",false);
		ironMan->GetComponent<BlackPearl::Transform>()->SetScale(glm::vec3(0.010));
		ironMan->GetComponent<BlackPearl::Transform>()->SetPosition({ 1.5f,-1.0f,1.0f });
		ironMan->GetComponent<BlackPearl::Transform>()->SetRotation({ 0.0f,0.0f,0.0f });

		m_BackGroundObjsList.push_back(ironMan);


		BlackPearl::Object* cube = CreateCube();
		cube->GetComponent<BlackPearl::Transform>()->SetPosition({ 0.0f,-3.0f,0.0f });
		m_BackGroundObjsList.push_back(cube);

		BlackPearl::Object* cube1 = CreateCube();
		cube1->GetComponent<BlackPearl::Transform>()->SetPosition({ -2.0f,-3.0f,0.0f });
		m_BackGroundObjsList.push_back(cube1);

		/*create pointlights*/
		BlackPearl::Object* light = CreateLight(BlackPearl::LightType::PointLight);
		light->GetComponent<BlackPearl::Transform>()->SetPosition({ 0.0,0.0,3.0 });

		//textures spheres
		std::shared_ptr<BlackPearl::Texture> RustalbedoTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::DiffuseMap, "assets/texture/pbr/rustSphere/rustediron2_basecolor.png"));
		std::shared_ptr<BlackPearl::Texture> RustaoTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::AoMap, "assets/texture/pbr/rustSphere/rustediron2_ao.png"));
		std::shared_ptr<BlackPearl::Texture> RustroughnessTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::RoughnessMap, "assets/texture/pbr/rustSphere/rustediron2_roughness.png"));
		std::shared_ptr<BlackPearl::Texture> RustmentallicTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::MentallicMap, "assets/texture/pbr/rustSphere/rustediron2_metallic.png"));
		std::shared_ptr<BlackPearl::Texture> RustnormalTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::NormalMap, "assets/texture/pbr/rustSphere/rustediron2_normal.png"));


		std::shared_ptr<BlackPearl::Texture> IronalbedoTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::DiffuseMap, "assets/texture/pbr/IronScuffed/Iron-Scuffed_basecolor.png"));
		std::shared_ptr<BlackPearl::Texture> IronaoTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::AoMap, "assets/texture/pbr/IronScuffed/Iron-Scuffed_ao.png"));
		std::shared_ptr<BlackPearl::Texture> IronroughnessTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::RoughnessMap, "assets/texture/pbr/IronScuffed/Iron-Scuffed_roughness.png"));
		std::shared_ptr<BlackPearl::Texture> IronmentallicTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::MentallicMap, "assets/texture/pbr/IronScuffed/Iron-Scuffed_metallic.png"));
		std::shared_ptr<BlackPearl::Texture> IronnormalTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::NormalMap, "assets/texture/pbr/IronScuffed/Iron-Scuffed_normal.png"));

		std::shared_ptr<BlackPearl::Texture> StonealbedoTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::DiffuseMap, "assets/texture/pbr/cobblestone/cobblestone-curved_2_albedo.png"));
		std::shared_ptr<BlackPearl::Texture> StoneaoTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::AoMap, "assets/texture/pbr/cobblestone/cobblestone-curved_2_ao.png"));
		std::shared_ptr<BlackPearl::Texture> StoneroughnessTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::RoughnessMap, "assets/texture/pbr/cobblestone/cobblestone-curved_2_roughness.png"));
		std::shared_ptr<BlackPearl::Texture> StonementallicTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::MentallicMap, "assets/texture/pbr/cobblestone/cobblestone-curved_2_metallic.png"));
		std::shared_ptr<BlackPearl::Texture> StonenormalTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::NormalMap, "assets/texture/pbr/cobblestone/cobblestone-curved_2_normal-dx.png"));

		std::shared_ptr<BlackPearl::Texture> PlasticalbedoTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::DiffuseMap, "assets/texture/pbr/plasticSphere/scuffed-plastic4-alb.png"));
		std::shared_ptr<BlackPearl::Texture> PlasticaoTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::AoMap, "assets/texture/pbr/plasticSphere/scuffed-plastic-ao.png"));
		std::shared_ptr<BlackPearl::Texture> PlasticroughnessTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::RoughnessMap, "assets/texture/pbr/plasticSphere/scuffed-plastic-rough.png"));
		std::shared_ptr<BlackPearl::Texture> PlasticmentallicTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::MentallicMap, "assets/texture/pbr/plasticSphere/scuffed-plastic-metal.png"));
		std::shared_ptr<BlackPearl::Texture> PlasticnormalTexture(DBG_NEW BlackPearl::Texture(BlackPearl::Texture::Type::NormalMap, "assets/texture/pbr/plasticSphere/scuffed-plastic-normal.png"));

		m_SphereObjIron->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(IronnormalTexture);
		m_SphereObjIron->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(IronalbedoTexture);
		m_SphereObjIron->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(IronaoTexture);
		m_SphereObjIron->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(IronroughnessTexture);
		m_SphereObjIron->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(IronmentallicTexture);
		m_SphereObjIron->GetComponent<BlackPearl::Transform>()->SetPosition({ 2,0,0 });

		m_SphereObjRust->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(RustnormalTexture);
		m_SphereObjRust->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(RustalbedoTexture);
		m_SphereObjRust->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(RustaoTexture);
		m_SphereObjRust->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(RustroughnessTexture);
		m_SphereObjRust->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(RustmentallicTexture);

		m_SphereObjRust->GetComponent<BlackPearl::Transform>()->SetPosition({ 5,0,0 });

		m_SphereObjStone->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(StonenormalTexture);
		m_SphereObjStone->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(StonealbedoTexture);
		m_SphereObjStone->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(StoneaoTexture);
		m_SphereObjStone->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(StoneroughnessTexture);
		m_SphereObjStone->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(StonementallicTexture);

		m_SphereObjStone->GetComponent<BlackPearl::Transform>()->SetPosition({ -5,0,0 });

		m_SphereObjPlastic->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(PlasticnormalTexture);
		m_SphereObjPlastic->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(PlasticalbedoTexture);
		m_SphereObjPlastic->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(PlasticaoTexture);
		m_SphereObjPlastic->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(PlasticroughnessTexture);
		m_SphereObjPlastic->GetComponent<BlackPearl::MeshRenderer>()->SetTextures(PlasticmentallicTexture);

		m_SphereObjPlastic->GetComponent<BlackPearl::Transform>()->SetPosition({ -10.0,0,0 });
		
		m_SphereObjsList.push_back(m_SphereObjIron);
		m_SphereObjsList.push_back(m_SphereObjStone);
		m_SphereObjsList.push_back(m_SphereObjPlastic);
		m_SphereObjsList.push_back(m_SphereObjRust);


		
		/*Draw CubeMap from hdrMap and Create environment IrrdianceMap*/
		m_IBLProbesRenderer->Init(m_SHImageQuadObj,m_BrdfLUTQuadObj,  *GetLightSources(), m_BackGroundObjsList, m_LightProbes);
		//m_IBLProbesRenderer->Render(GetLightSources(), m_BackGroundObjsList, m_LightProbes, m_SkyBoxObj1);

		//glViewport(0, 0, BlackPearl::Configuration::WindowWidth, BlackPearl::Configuration::WindowHeight);

	}

	virtual ~IBLProbesRenderingLayer() {

		DestroyObjects();
		delete m_BasicRenderer;
		delete m_IBLProbesRenderer;

	}
	void Hello() {
		GE_CORE_INFO("Hello");
	}
	void OnUpdate(BlackPearl::Timestep ts) override {


		InputCheck(ts);

		/*render scene*/
		BlackPearl::RenderCommand::SetClearColor(m_BackgroundColor);
		BlackPearl::Renderer::BeginScene(*(m_MainCamera->GetObj()->GetComponent<BlackPearl::PerspectiveCamera>()), *GetLightSources());


		if (BlackPearl::Input::IsKeyPressed(BP_KEY_U)) {

			/*IBLProbes rendering*/

			//std::vector<std::thread>probeThreads;
			//	for (auto it = probes.begin(); it != probes.end(); it++)
			//m_IBLProbesRenderer->ReUpdateProbes();

			m_IBLProbesRenderer->Render(GetLightSources(), m_BackGroundObjsList, m_LightProbes, m_SkyBoxObj1);


		}

		milliseconds currentTimeMs = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		double runtime = currentTimeMs.count()-m_StartTimeMs.count();
		


		
		
		glViewport(0, 0, BlackPearl::Configuration::WindowWidth, BlackPearl::Configuration::WindowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_AnimatedModelRenderer->Render(m_AnimatedModel, runtime / 1000.0f);

		m_BasicRenderer->RenderScene(m_BackGroundObjsList, GetLightSources());
		m_IBLProbesRenderer->RenderProbes(m_LightProbes);
		glDepthFunc(GL_LEQUAL);

		m_IBLProbesRenderer->DrawObject(m_SkyBoxObj1);
		glDepthFunc(GL_LESS);


		
		m_IBLProbesRenderer->RenderSpecularObjects(GetLightSources(), m_SphereObjsList, m_LightProbes);

		/*draw BRDFLUTTextureID in a quad*/
		/*glViewport(0, 0, 120, 120);
		m_DebugQuadShader->Bind();
		m_DebugQuad->GetComponent<BlackPearl::MeshRenderer>()->SetShaders(m_DebugQuadShader);
		m_DebugQuadShader->SetUniform1i("u_BRDFLUTMap", 6);
		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_2D, m_IBLProbesRenderer->GetSpecularBrdfLUTTexture()->GetRendererID());
		m_BasicRenderer->DrawObject(m_DebugQuad, m_DebugQuadShader);
		m_IBLProbesRenderer->GetSpecularBrdfLUTTexture()->UnBind();*/

	//	glViewport(0, 0, BlackPearl::Configuration::WindowWidth, BlackPearl::Configuration::WindowHeight);


	}

	void OnAttach() override {


	}
	
private:
	//Scene
	
	//BlackPearl::Object* m_DebugQuad = nullptr;

	BlackPearl::Object* m_SphereObj = nullptr;
	BlackPearl::Object* m_SphereObjIron = nullptr;
	BlackPearl::Object* m_SphereObjRust = nullptr;
	BlackPearl::Object* m_SphereObjStone = nullptr;
	BlackPearl::Object* m_SphereObjPlastic = nullptr;
	std::vector<BlackPearl::Object*> m_SphereObjsList;
	BlackPearl::Object* m_SkyBoxObj1 = nullptr;

	BlackPearl::Object* m_BrdfLUTQuadObj = nullptr;
	BlackPearl::Object* m_SHImageQuadObj = nullptr;
	BlackPearl::Object* m_DebugQuad = nullptr;

	/*Animation model*/
	BlackPearl::Object* m_AnimatedModel = nullptr;
	int m_Rows = 4;
	int m_Colums = 4;
	float m_Spacing = 1.5;

	

	//Shader
	std::shared_ptr<BlackPearl::Shader> m_BackGroundShader;
	std::shared_ptr<BlackPearl::Shader> m_DebugQuadShader;
	//Renderer
	BlackPearl::IBLProbesRenderer* m_IBLProbesRenderer;
	BlackPearl::BasicRenderer* m_BasicRenderer;
	BlackPearl::AnimatedModelRenderer* m_AnimatedModelRenderer;


	/* Probes */
	std::vector<BlackPearl::LightProbe*> m_LightProbes;
	std::thread m_Threads[10];

	/*Time*/
	std::chrono::milliseconds m_StartTimeMs;
};
