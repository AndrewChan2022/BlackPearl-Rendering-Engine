#include <BlackPeral.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "BlackPearl/Application.h"
#include "imgui/imgui.h"
#include "BlackPearl/System/System.h"
#include <glm/gtc/type_ptr.hpp>

class EntityTestLayer :public BlackPearl::Layer {
public:

	EntityTestLayer(const std::string& name, BlackPearl::SystemManager *systemManager, BlackPearl::EntityManager *entityManager)
		: Layer(name, systemManager, entityManager)
	{


		BlackPearl::Entity *entity = entityManager->CreateEntity();
		m_CameraObj = new BlackPearl::Object(entity->GetEntityManager(), entity->GetId());
		std::shared_ptr<BlackPearl::Camera> cameraComponent(m_CameraObj->AddComponent<BlackPearl::PerspectiveCamera>());


		cameraComponent->SetPosition(glm::vec3(0.0f, 0.0f, 8.0f));
		m_CameraPosition = cameraComponent->GetPosition();
		m_CameraRotation.Yaw = cameraComponent->Yaw();
		m_CameraRotation.Pitch = cameraComponent->Pitch();


		glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-2.7f,  3.0f, -7.5f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		};
		std::vector<BlackPearl::Light::Props> pointLightProps = {
		{{0.2f, 0.2f, 0.2f},{ 1.0f, 0.2f, 0.1f},{1.0f, 1.0f, 1.0f}},
		{{0.2f, 0.2f, 0.2f},{ 0.2f, 1.0f, 0.1f},{1.0f, 1.0f, 1.0f}},
		{{0.2f, 0.2f, 0.2f},{ 0.1f, 0.2f, 1.0f},{1.0f, 1.0f, 1.0f}},
		{{0.2f, 0.2f, 0.2f},{ 0.1f, 0.5f, 0.5f},{1.0f, 1.0f, 1.0f}}

		};
		m_LightObjs.resize(pointLightProps.size());
		for (int i = 0; i < pointLightProps.size(); i++)
		{
			BlackPearl::Entity* lightEntity = entityManager->CreateEntity();
			m_LightObjs[i] = new BlackPearl::Object(lightEntity->GetEntityManager(), lightEntity->GetId());


			std::shared_ptr<BlackPearl::Light> lightComponent = m_LightObjs[i]->AddComponent<BlackPearl::PointLight>();

			//lightComponent.reset();

			lightComponent->SetProps(pointLightProps[i]);
			std::dynamic_pointer_cast<BlackPearl::PointLight>(lightComponent)->SetPosition(pointLightPositions[i]);
			std::dynamic_pointer_cast<BlackPearl::PointLight>(lightComponent)->SetAttenuation(BlackPearl::PointLight::Attenuation(3250));

			m_LightSources.AddLight(std::move(lightComponent));
		}


		/*m_Shader.reset(new BlackPearl::Shader("assets/shaders/IronMan.glsl"));
		m_Shader->Bind();
		m_Model.reset(new BlackPearl::Model("assets/models/IronMan/IronMan.obj"));*/
		std::shared_ptr<BlackPearl::Shader>IronManShader(new BlackPearl::Shader("assets/shaders/IronMan.glsl"));
		IronManShader->Bind();
		std::shared_ptr<BlackPearl::Model> IronManModel(new BlackPearl::Model("assets/models/IronMan/IronMan.obj", IronManShader));
		BlackPearl::Entity *ironManEntity = entityManager->CreateEntity();
		m_IronManObj = new BlackPearl::Object(ironManEntity->GetEntityManager(), ironManEntity->GetId());

		std::shared_ptr<BlackPearl::Transform> TransformComponent(m_IronManObj->AddComponent<BlackPearl::Transform>());
		TransformComponent->SetPosition({ 0.0f, -1.75f, 0.0f });
		TransformComponent->SetRotation({ 0.0,180.0,0.0 });
		TransformComponent->SetScale({ 0.01f, 0.01f, 0.01f });

		std::shared_ptr<BlackPearl::MeshRenderer> meshRendererComponent(m_IronManObj->AddComponent<BlackPearl::MeshRenderer>(IronManModel, TransformComponent->GetTransformMatrix()));



		BlackPearl::Renderer::Init();


	}

	virtual ~EntityTestLayer() {

		for (auto lightObj : m_LightObjs)
			delete lightObj;
		delete m_CameraObj;

	}
	void OnUpdate(BlackPearl::Timestep ts) override {


		InputCheck(ts);

		// render
		BlackPearl::RenderCommand::SetClearColor(m_BackgroundColor);
		//m_Shader->Bind();

		BlackPearl::Renderer::BeginScene(*(m_CameraObj->GetComponent<BlackPearl::PerspectiveCamera>()), m_LightSources);
		m_IronManObj->GetComponent<BlackPearl::MeshRenderer>()->DrawModel();

	}

	void OnImguiRender() override {

		
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Suqare Color", glm::value_ptr(m_BackgroundColor));
		ImGui::End();


	}
	void OnAttach() override {


	}
	void InputCheck(float ts)
	{
		auto cameraComponent = m_CameraObj->GetComponent<BlackPearl::PerspectiveCamera>();
		if (BlackPearl::Input::IsKeyPressed(BP_KEY_W)) {
			m_CameraPosition += cameraComponent->Front() * m_CameraMoveSpeed * ts;
		}
		else if (BlackPearl::Input::IsKeyPressed(BP_KEY_S)) {
			m_CameraPosition -= cameraComponent->Front()* m_CameraMoveSpeed * ts;
		}
		if (BlackPearl::Input::IsKeyPressed(BP_KEY_A)) {
			m_CameraPosition -= cameraComponent->Right() * m_CameraMoveSpeed * ts;
		}
		else if (BlackPearl::Input::IsKeyPressed(BP_KEY_D)) {
			m_CameraPosition += cameraComponent->Right() * m_CameraMoveSpeed * ts;
		}
		if (BlackPearl::Input::IsKeyPressed(BP_KEY_E)) {
			m_CameraPosition -= cameraComponent->Up() * m_CameraMoveSpeed * ts;
		}
		else if (BlackPearl::Input::IsKeyPressed(BP_KEY_Q)) {
			m_CameraPosition += cameraComponent->Up() * m_CameraMoveSpeed * ts;
		}
		// ---------------------Rotation--------------------------------------

		float posx = BlackPearl::Input::GetMouseX();
		float posy = BlackPearl::Input::GetMouseY();
		if (BlackPearl::Input::IsMouseButtonPressed(BP_MOUSE_BUTTON_RIGHT)) {


			if (BlackPearl::Input::IsFirstMouse()) {
				BlackPearl::Input::SetFirstMouse(false);
				m_LastMouseX = posx;
				m_LastMouseY = posy;
			}
			float diffx = posx - m_LastMouseX;
			float diffy = -posy + m_LastMouseY;

			m_LastMouseX = posx;
			m_LastMouseY = posy;
			m_CameraRotation.Yaw += diffx * m_CameraRotateSpeed * ts;
			m_CameraRotation.Pitch += diffy * m_CameraRotateSpeed * ts;

			if (m_CameraRotation.Pitch > 89.0f)
				m_CameraRotation.Pitch = 89.0f;
			if (m_CameraRotation.Pitch < -89.0f)
				m_CameraRotation.Pitch = -89.0f;

			cameraComponent->SetRotation(m_CameraRotation.Yaw, m_CameraRotation.Pitch);

		}
		else {

			m_LastMouseX = posx;//lastMouseʱ�̼�¼��ǰ����λ�ã���ֹ�ٴε���Ҽ�ʱ������������
			m_LastMouseY = posy;
		}


		cameraComponent->SetPosition(m_CameraPosition);
	}
private:

	std::vector<BlackPearl::Object*> m_LightObjs;
	BlackPearl::Object *m_CameraObj;
	BlackPearl::Object *m_IronManObj;
	BlackPearl::LightSources m_LightSources;
	//std::shared_ptr<BlackPearl::Light> m_LightComponent;
	//std::shared_ptr<BlackPearl::Camera> m_CameraComponent;

	glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
	struct CameraRotation {
		float Yaw;
		float Pitch;

	};
	CameraRotation m_CameraRotation;
	float m_LastMouseX;
	float m_LastMouseY;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotateSpeed = 9.0f;

	glm::vec4 m_BackgroundColor = { 0.0f,0.0f,0.0f,0.0f };


};

class SandBox :public BlackPearl::Application {
public:
	SandBox() {
		//PushLayer(new ExampleLayer("ExampleLayer"));
		BlackPearl::EntityManager * entityManager = DBG_NEW BlackPearl::EntityManager();
		BlackPearl::Layer* layer = DBG_NEW EntityTestLayer("Entity Layer", nullptr, entityManager);
		PushLayer(layer);
	}
	virtual ~SandBox() = default;

};

BlackPearl::Application* BlackPearl::CreateApplication() {

	return DBG_NEW SandBox();

}

