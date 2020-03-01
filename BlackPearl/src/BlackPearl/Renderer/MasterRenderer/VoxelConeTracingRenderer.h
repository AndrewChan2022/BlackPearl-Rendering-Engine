#pragma once
#include "BlackPearl/Renderer/MasterRenderer/BasicRenderer.h"
#include "BlackPearl/Renderer/Material/Texture3D.h"
#include "BlackPearl/Renderer/Shader/Shader.h"
#include "BlackPearl/Renderer/Buffer.h"
#include "BlackPearl/Renderer/Material/TextureImage2D.h"
namespace BlackPearl {
	class VoxelConeTracingRenderer:public BasicRenderer
	{
	public:
		enum RenderingMode {
			VOXELIZATION_VISUALIZATION = 0, // Voxelization visualization.
			VOXEL_CONE_TRACING = 1			// Global illumination using voxel cone tracing.
		};
		VoxelConeTracingRenderer();
		~VoxelConeTracingRenderer();

		void Init(unsigned int viewportWidth,unsigned int viewportHeight, Object * quadObj, Object * cubeObj);//, Object* debugQuadObj
		void InitVoxelization();
		void InitVoxelVisualization(unsigned int viewportWidth, unsigned int viewportHeight);
		void Voxilize(const std::vector<Object*>&objs,
			bool clearVoxelizationFirst = true);
		void Render(
			const std::vector<Object*>&objs, 
			const LightSources* lightSources,
			unsigned int viewportWidth,
			unsigned int viewportHeight,
			RenderingMode reneringMode = RenderingMode::VOXELIZATION_VISUALIZATION
		);
		void RenderVoxelVisualization(const std::vector<Object*>&objs,  unsigned int viewportWidth, unsigned int viewportHeight);
		void RenderScene(const std::vector<Object*>&objs, unsigned int viewportWidth, unsigned int viewportHeight);


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
		void DrawFrontBackFaceOfCube(Object* debugQuadObj);
	private:
		Texture3D* m_VoxelTexture = nullptr;
		unsigned int m_VoxelTextureSize = 256;// 256;// 64;


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
		bool m_Shadows = true;
		bool m_IndirectDiffuseLight = true;
		bool m_IndirectSpecularLight = true;
		bool m_DirectLight = true;


		std::shared_ptr<Shader> m_VoxelizationShader;
		std::shared_ptr<Shader> m_WorldPositionShader;
		std::shared_ptr<Shader> m_VoxelVisualizationShader;
		std::shared_ptr<Shader> m_VoxelConeTracingShader;

		/*Debug function*/
		std::shared_ptr<Shader> m_VoxelizationTestShader;
		std::shared_ptr<Shader> m_FrontBackCubeTestShader;


		std::shared_ptr<FrameBuffer> m_FrameBuffer1;
		std::shared_ptr<FrameBuffer> m_FrameBuffer2;

		Object * m_QuadObj = nullptr;//������ʾ���ػ���� �� voxel Visualization
		Object * m_CubeObj = nullptr; //�������ػ���Ⱦ��Χ

		Object* m_DebugQuadObj = nullptr; //darw front face and back face of cube


		bool m_IsInitialize = false;



		/*u_CubeSize*/
		//glm::vec3 m_CubeSize{ 10.0f };

		std::shared_ptr< TextureImage2D> m_CubeBackPosTexture;

	};

}
