#include "pch.h"
#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Material/Texture.h"
#include "Renderer/CameraComponent/PerspectiveCamera.h"
#include "Renderer/Renderer.h"
#include "Renderer/Buffer.h"
#include "Core.h"
#include "Input.h"
#include "Event/Event.h"
#include "Event/MouseEvent.h"
#include "Renderer/LightComponent/ParallelLight.h"
#include "Renderer/LightComponent/PointLight.h"
#include "Renderer/LightComponent/SpotLight.h"
#include "ImGui/ImGuiLayer.h"
#include "BlackPearl/System/System.h"
#include "BlackPearl/Entity/Entity.h"
namespace BlackPearl {

	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		GE_ASSERT(!s_Instance, "Application's Instance already exist!")
		s_Instance = this;
		m_Window.reset(DBG_NEW Window());
		m_Window->SetCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		EntityManager *ImGuiEntityMgr = DBG_NEW EntityManager();
		SystemManager *ImGuiSystemMgr = DBG_NEW SystemManager(*ImGuiEntityMgr);
		m_ImGuiLayer = DBG_NEW ImGuiLayer("ImGuiLayer",ImGuiSystemMgr,ImGuiEntityMgr);
		PushOverLayer(m_ImGuiLayer);

	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		while (!glfwWindowShouldClose(m_Window->GetNativeWindow())) {
			float time = (float)glfwGetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (glfwGetKey(m_Window->GetNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(m_Window->GetNativeWindow(), true);

			for (Layer* layer : m_LayerStack) {

				layer->OnUpdate(ts);

			}
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImguiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

		}


		//glm::vec3 cubePositions[] = {
		//glm::vec3(0.0f,  0.0f,  0.0f),
		//glm::vec3(2.0f,  5.0f, -15.0f),
		//glm::vec3(-1.5f, -2.2f, -2.5f),
		//glm::vec3(-3.8f, -2.0f, -12.3f),
		//glm::vec3(2.4f, -0.4f, -3.5f),
		//glm::vec3(-1.7f,  3.0f, -7.5f),
		//glm::vec3(1.3f, -2.0f, -2.5f),
		//glm::vec3(1.5f,  2.0f, -2.5f),
		//glm::vec3(1.5f,  0.2f, -1.5f),
		//glm::vec3(-1.3f,  1.0f, -1.5f)
		//};

		//render loop
		glfwTerminate();
	}



	void Application::OnEvent(Event& event)
	{
		EventDispacher dispacher(event);

		dispacher.Dispatch<MouseMovedEvent>(std::bind(&Application::OnCameraRotate, this, std::placeholders::_1));

	}


	bool Application::OnCameraRotate(MouseMovedEvent & e)
	{
		float posx = e.GetMouseX();
		float posy = e.GetMouseY();

		return true;
	}

	void Application::PushLayer(Layer * layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer * overlay)
	{
		m_LayerStack.PushOverLay(overlay);
		overlay->OnAttach();
	}
}