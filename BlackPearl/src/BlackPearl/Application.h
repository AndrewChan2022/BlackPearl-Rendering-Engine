#pragma once
#include "BlackPearl/Renderer/Shader/Shader.h"
#include <memory>
#include "Window.h"
#include "Renderer/Material/Texture.h"
#include "Component/CameraComponent/Camera.h"
#include "Renderer/VertexArray.h"
#include "BlackPearl/Timestep/Timestep.h"
#include "Event/MouseEvent.h"
#include "Component/LightComponent/Light.h"
#include "Component/LightComponent/LightSources.h"
#include "Renderer/Mesh/Mesh.h"
#include "Renderer/Model/Model.h"
#include "LayerScene/Layer.h"
#include "LayerStack.h"
#include "BlackPearl/Event/Event.h"
#include "BlackPearl/ImGui/ImGuiLayer.h"
#include "BlackPearl/Scene/Scene.h"
#include "BlackPearl/ObjectManager/ObjectManager.h"
#include "BlackPearl/Entity/Entity.h"
#include "BlackPearl/RHI/DynamicRHI.h"
#include <chrono>
using namespace std::chrono;
namespace BlackPearl {


	class Application
	{
	public:
		struct AppConf {
			HINSTANCE hInstance;
			int nShowCmd;
			std::string renderer;
		};
		Application(HINSTANCE hInstance, int nShowCmd, const std::string& renderer);
		virtual ~Application();

		inline static Application &Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		Scene* GetScene() { return m_CurrentScene; }
		AppConf GetAppConf() const { return m_AppConf; }
		void Init();
		void Run();
		void OnEvent(Event &event);
		static double s_AppFPS;
		static double s_AppAverageFPS;
		static bool IsFullscreen();
		static void SetWindowZorderToTopMost(bool setToTopMost);

	private:
		bool ShouldCloseWindow();
		bool OnCameraRotate(MouseMovedEvent&e);
		bool OnWindowClose();
	
	
	private:
		static Application* s_Instance; //TODO::可以不delete,或者改为 unique_ptr
		float m_LastFrameTime = 0.0f;

	private:
		DynamicRHI* m_DynamicRHI;
		Window* m_Window;
		AppConf m_AppConf;

		double m_StartTimeMs;
		long long m_FrameNum = 0;
		Scene* m_CurrentScene = nullptr;
		long long m_TotalFrameNum = 0;
		double m_TotalSecond = 0;

	};
	//To be define in a client
	Application * CreateApplication(HINSTANCE hInstance, int nShowCmd);
}