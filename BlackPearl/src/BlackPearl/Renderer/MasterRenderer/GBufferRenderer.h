#pragma once
#include "BlackPearl/Renderer/Buffer.h"
#include "BlackPearl/Object/Object.h"
#include "BlackPearl/Renderer/MasterRenderer/BasicRenderer.h"
#include "BlackPearl/Config.h"
namespace BlackPearl {
	class GBufferRenderer:public BasicRenderer
	{
	public:
		GBufferRenderer();
		/* ScreenQuad render scene to the screen*/
		void Init(Object* ScreenQuad, Object* surroundSphere);
		void Render(std::vector<Object*> objects, Object* gBufferDebugQuad, LightSources* lightSources);
		void DrawGBuffer(Object* gBufferDebugQuad);
		float CalculateSphereRadius(Object* pointLight);


	private:
		bool m_IsInitialized = false;

		Object* m_ScreenQuad= nullptr;
		/* pointLight ��Χ��,ֻ��Ⱦ���е�fragment */
		Object* m_SurroundSphere = nullptr;

		std::shared_ptr<GBuffer> m_GBffer;
		/* write pos,normal,color to gBuffer */
		std::shared_ptr<Shader> m_GBufferShader;

		/* ����Debug--������Χ�� */
		std::shared_ptr<Shader> m_SphereDeBugShader;


		/* draw lights according to gBuffer data */
		//�ֿ�����pass����Ⱦ-->ȥ��if��֧��GPU��if,for���Ѻ�
		std::shared_ptr<Shader> m_PointLightPassShader;
		std::shared_ptr<Shader> m_DirectionLightPassShader;

		unsigned int m_TextureWidth = Configuration::WindowWidth;
		unsigned int m_TexxtureHeight = Configuration::WindowHeight;

		/* 0.0f <m_AttenuationItensity< 256.0f ,m_AttenuationItensityԽ��˥��Խ�죬��Χ��m_SurroundSphere �뾶ԽС */
		float m_AttenuationItensity = 100.0f;
	};

}

