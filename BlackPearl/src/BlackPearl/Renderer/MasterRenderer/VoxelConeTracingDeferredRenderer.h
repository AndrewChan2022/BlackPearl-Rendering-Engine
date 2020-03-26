#pragma once
#include "BlackPearl/Renderer/MasterRenderer/BasicRenderer.h"
#include "BlackPearl/Renderer/Material/Texture3D.h"
#include "BlackPearl/Renderer/Shader/Shader.h"
#include "BlackPearl/Renderer/Buffer.h"
#include "BlackPearl/Renderer/Material/TextureImage2D.h"
#include "BlackPearl/Renderer/Buffer.h"
namespace BlackPearl {
	class VoxelConeTracingDeferredRenderer :public BasicRenderer
	{
	public:
		enum RenderingMode {
			VOXELIZATION_VISUALIZATION = 0, // Voxelization visualization.
			VOXEL_CONE_TRACING = 1			// Global illumination using voxel cone tracing.
		};
		VoxelConeTracingDeferredRenderer();
		~VoxelConeTracingDeferredRenderer();

		void Init(unsigned int viewportWidth, unsigned int viewportHeight, Object* quadObj, Object* quadGbufferObj, 
			Object* brdfLUTQuadObj, Object* m_quadFinalScreenObj , Object* surroundSphere,Object* cubeObj);//, Object* debugQuadObj
		void InitVoxelization();
		void InitVoxelVisualization(unsigned int viewportWidth, unsigned int viewportHeight);
		void Voxilize(const std::vector<Object*>& objs,  Object* skybox,
			bool clearVoxelizationFirst = true);
		void Render(
			Camera* camera,
			const std::vector<Object*>& objs,
			const LightSources* lightSources,
			unsigned int viewportWidth,
			unsigned int viewportHeight, Object* skybox,
			RenderingMode reneringMode = RenderingMode::VOXELIZATION_VISUALIZATION
		);
		void RenderVoxelVisualization(Camera* camera, const std::vector<Object*>& objs, unsigned int viewportWidth, unsigned int viewportHeight);
		void RenderScene(const std::vector<Object*>& objs, const LightSources* lightSources, 
			unsigned int viewportWidth, unsigned int viewportHeight, Object* skybox);

		void RenderSpecularBRDFLUTMap();

		/* GBuffer */
		void RenderGBuffer(const std::vector<Object*>& objs, Object* skybox);
		void DrawGBuffer(Object* gBufferDebugQuad);

		/* ���object���ػ� */
		/*void Voxilize( Object* obj,bool clearVoxelizationFirst = true);
		void Render(
			 Object* obj,
			const LightSources* lightSources,
			unsigned int viewportWidth,
			unsigned int viewportHeight,
			glm::vec3 cameraPos,
			RenderingMode reneringMode = RenderingMode::VOXELIZATION_VISUALIZATION
		);
		void RenderVoxelVisualization( Object* obj, unsigned int viewportWidth, unsigned int viewportHeight);
		void RenderScene( Object* obj, unsigned int viewportWidth, unsigned int viewportHeight);*/



		/*Debug function*/
		void VoxelizeTest(const std::vector<Object*>& objs);
		/* Settings */
		static bool s_Shadows;
		static bool s_IndirectDiffuseLight;
		static bool s_IndirectSpecularLight;
		static bool s_DirectLight;
		static float s_GICoeffs;
		static bool s_VoxelizeNow;
		static bool s_HDR;
		static bool s_GuassianHorizontal;
		static bool s_GuassianVertical;
		static bool s_ShowBlurArea;
		static float s_SpecularBlurThreshold;
	private:

		void SetgBufferTextureUniforms();

		int m_BlurCoeffs = 8;

		Texture3D* m_VoxelTexture = nullptr;
		unsigned int m_VoxelTextureSize = 256;// 256;// 64;

		unsigned int m_ScreenWidth = Configuration::WindowWidth;
		unsigned int m_ScreenHeight = Configuration::WindowHeight;
		// ----------------
		// Voxelization.
		// ----------------

		bool m_AutomaticallyRegenerateMipmap = true;
		bool m_RegenerateMipmapQueued = true;
		bool m_AutomaticallyVoxelize = true;
		bool m_VoxelizationQueued = true;
		int  m_VoxelizationSparsity = 1; // Number of ticks between mipmap generation. 
		// (voxelization sparsity gives unstable framerates, so not sure if it's worth it in interactive applications.)
		int m_TicksSinceLastVoxelization = m_VoxelizationSparsity;

		// ----------------
		// Rendering.
		// ----------------
		std::shared_ptr<Shader> m_VoxelizationShader;
		std::shared_ptr<Shader> m_WorldPositionShader;
		std::shared_ptr<Shader> m_VoxelVisualizationShader;

		std::shared_ptr<Shader> m_VCTAmbientGIShader;//deferred ambient GI pass
		std::shared_ptr<Shader> m_VCTPointLightShader;//deferred pointlight pass

		std::shared_ptr<Shader> m_GuassianFilterVerticalShader;
		std::shared_ptr<Shader> m_GuassianFilterHorizontalShader;

		std::shared_ptr<Shader> m_FinalScreenShader;

		// ----------------
		// PBR BRDF LUT render.
		// ----------------
		std::shared_ptr<Shader> m_SpecularBRDFLutShader;
		/*ֻ����һ��*/
		std::shared_ptr<Texture> m_SpecularBrdfLUTTexture = nullptr;
		Object* m_BrdfLUTQuadObj = nullptr;//pbr brdf LUT map render

		/*Debug function*/
		std::shared_ptr<Shader> m_VoxelizationTestShader;
		std::shared_ptr<Shader> m_FrontBackCubeTestShader;


		// ----------------
		// Object
		// ----------------
		Object* m_QuadObj = nullptr;//������ʾ���ػ���� �� voxel Visualization
		Object* m_CubeObj = nullptr; //�������ػ���Ⱦ��Χ
		Object* m_DebugQuadObj = nullptr; //darw front face and back face of cube
		Object* m_QuadFinalScreenObj = nullptr;
		Object* m_SurroundSphere = nullptr;
		// ----------------
		// GBuffer
		// ----------------
		std::shared_ptr<GBuffer> m_GBuffer;
		std::shared_ptr<Shader> m_GBufferShader;
		Object* m_QuadGbufferObj = nullptr;//������ʾ���ػ���� �� voxel Visualization

		// ----------------
		// FrameBuffer which render scene to 
		// ----------------
		std::shared_ptr<FrameBuffer> m_FrameBuffer;
		std::shared_ptr<Texture> m_PostProcessTexture;

		std::shared_ptr<FrameBuffer> m_BlurHoriFrameBuffer;
		std::shared_ptr<Texture> m_BlurHTexture;

		std::shared_ptr<FrameBuffer> m_BlurVertFrameBuffer;
		std::shared_ptr<Texture> m_BlurVTexture;

		bool m_IsInitialize = false;





	};

}