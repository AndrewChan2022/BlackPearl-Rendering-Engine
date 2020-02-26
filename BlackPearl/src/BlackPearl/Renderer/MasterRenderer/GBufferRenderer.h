#pragma once
#include "BlackPearl/Renderer/Buffer.h"
#include "BlackPearl/Object/Object.h"
#include "BlackPearl/Renderer/MasterRenderer/BasicRenderer.h"
#include "BlackPearl/Config.h"
#include "BlackPearl/LightProbes/LightProbes.h"
namespace BlackPearl {
	class GBufferRenderer:public BasicRenderer
	{
	public:
		GBufferRenderer();
		/* ScreenQuad render scene to the screen*/
		void Init(Object* ScreenQuad, Object* surroundSphere, Object* GIQuad);
		void Render(std::vector<Object*> objects, Object* gBufferDebugQuad, LightSources* lightSources);
		void DrawGBuffer(Object* gBufferDebugQuad);
		float CalculateSphereRadius(Object* pointLight);
		/* probes for ambient light render (GI) */
		void RenderSceneWithGBufferAndProbes(std::vector<Object*> objects, std::vector<Object*> backGroundObjs, Object* gBufferDebugQuad, LightSources* lightSources,
			std::vector<LightProbe*> probes, std::shared_ptr<Texture> specularBrdfLUTTexture, Object* skyBox);
		std::vector<LightProbe*> FindKnearProbes(glm::vec3 objPos, std::vector<LightProbe*> probes);

	private:
		bool m_IsInitialized = false;

		Object* m_FinalScreenQuad = nullptr;
		/* 用于渲染环境全局光照 AmbientGI pass */
		Object* m_GIQuad= nullptr;
		/* pointLight 包围球,只渲染球中的fragment  pontlight pass */
		Object* m_SurroundSphere = nullptr;



		/**** buffer ****/
		std::shared_ptr<GBuffer> m_GBffer;
		/* 用作 tone mapping 和 hdr 后期处理 */
		std::shared_ptr<FrameBuffer> m_HDRFrameBuffer;
		std::shared_ptr<Texture> m_HDRPostProcessTexture;


		/**** shader ****/
		/* write pos,normal,color to gBuffer */
		std::shared_ptr<Shader> m_GBufferShader;

		/* 用于Debug--画出包围球 */
		std::shared_ptr<Shader> m_SphereDeBugShader;

		/* draw lights according to gBuffer data */
		//分开多个pass来渲染-->去除if分支，GPU对if,for不友好
		std::shared_ptr<Shader> m_AmbientGIPassShader;
		std::shared_ptr<Shader> m_PointLightPassShader;
		std::shared_ptr<Shader> m_DirectionLightPassShader;

		std::shared_ptr<Shader> m_FinalScreenShader;


		unsigned int m_TextureWidth = Configuration::WindowWidth;
		unsigned int m_TexxtureHeight = Configuration::WindowHeight;

		/* 0.0f <m_AttenuationItensity< 256.0f ,m_AttenuationItensity越大，衰减越快，包围球m_SurroundSphere 半径越小 */
		float m_AttenuationItensity = 100.0f;

		/* m_K near probes for specular objs rendering */
		/*注意 物体blending 多个(m_K个) probes的diffuse SH,但只取最近的一个Probe的specular Map*/
		unsigned int m_K = 2;

	};

}

