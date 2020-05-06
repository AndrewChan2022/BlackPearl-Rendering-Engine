#pragma once
#include "BlackPearl/Component/LightComponent/Light.h"
#include "BlackPearl/LayerScene/Layer.h"
#include "BlackPearl/LayerStack.h"
#include "BlackPearl/ImGui/ImGuiLayer.h"
#include "BlackPearl/Timestep/Timestep.h"
#include "BlackPearl/Entity/Entity.h"
#include "BlackPearl/ObjectManager/ObjectManager.h"

namespace BlackPearl {

	class Scene {
	public:
		Scene() {
			m_ImGuiEntityMgr = DBG_NEW EntityManager();
			m_ImGuiObjectMgr = DBG_NEW ObjectManager(m_ImGuiEntityMgr);
			m_ImGuiLayer = DBG_NEW ImGuiLayer("ImGuiLayer", m_ImGuiObjectMgr);
			PushOverLayer(m_ImGuiLayer);
		}
		~Scene() {
			//Layerͨ��m_LayerStackɾ������Ҫ�ظ�ɾ����
			//GE_SAVE_DELETE(m_ImGuiLayer);
			//GE_SAVE_DELETE(m_ImGuiEntityMgr);
			//GE_SAVE_DELETE(m_ImGuiObjectMgr);

		};

		void OnUpdateLayers(Timestep ts);
		void PushLayer(Layer *layer);
		void PushOverLayer(Layer *overlay);

	private:

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer; //ÿ��Scene����һ��ImGuiLayer
		EntityManager* m_ImGuiEntityMgr;
		ObjectManager* m_ImGuiObjectMgr;
	};



}
