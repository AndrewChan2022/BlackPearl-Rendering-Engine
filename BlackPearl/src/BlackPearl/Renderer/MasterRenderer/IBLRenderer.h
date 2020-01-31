#pragma once
#include "BlackPearl/Renderer/Buffer.h"
#include "BlackPearl/Renderer/Material/HDRTexture.h"
#include "BlackPearl/Renderer/Material/CubeMapTexture.h"
#include"BlackPearl/Object/Object.h"
#include "BlackPearl/Renderer/Shader/Shader.h"
#include "BlackPearl/Renderer/MasterRenderer/BasicRenderer.h"
namespace BlackPearl {
	class IBLRenderer:public BasicRenderer
	{
	public:
		IBLRenderer();
		void Init(Object* hdrCubeObj, Object * brdfLUTQuad);
		~IBLRenderer();
		void RenderHdrMapToEnvironmentCubeMap();
		void RenderDiffuseIrradianceMap();

		//��Ԥ�ȼ���õ�BRDF�ԴֲڶȺ�����ǵ���ϵ�Ӱ���
		//���洢��һ��2D��������(LUT)��-->BRDF������ͼ
		//2D��������洢���Ƿ�������Ӧ��ϵ��(Rͨ������ƫ��ֵ(Gͨ��)
		void RenderSpecularPrefilterMap();
		//restore the result of BRDF convolution
		void RenderSpecularBRDFLUTMap();
		void RenderSpheres(Object* sphere);
		void RenderTextureSphere(Object* sphere);

		//draw brdfLUT quad (just for debug)
		void DrawBRDFLUTMap();
		void Render(std::vector<Object*>objs);
		std::shared_ptr<FrameBuffer> GetFrameBuffer() { return m_FrameBuffer; }
		unsigned int GetHdrCubeMapID() { return m_HdrCubeMapID; }
		unsigned int GetIrradianceCubeMap() { return m_IrradianceCubeMapID; }
		unsigned int GetPrefilterMapID() { return m_PrefilterCubeMapID; }
		unsigned int GetBRDFLUTTextureID() { return m_BRDFLUTTextureID; }


	private:
		std::shared_ptr<FrameBuffer> m_FrameBuffer;
		
	
		static int s_Width;
		static int s_Height;
		
		//Hdr Texture
		std::shared_ptr<HDRTexture>	m_HdrTexture;
	protected:
		//Shader
		std::shared_ptr<Shader>		m_HdrMapToCubeShader;//create environmentMap from hdr texture
		std::shared_ptr<Shader>		m_IBLShader; //scene renderer
		std::shared_ptr<Shader>		m_IrradianceShader;  //create diffuse irradianceCubeMap

		std::shared_ptr<Shader>		m_SpecularBRDFLutShader;  // brdf LUT shader
		std::shared_ptr<Shader>		m_SpecularPrefilterShader; //specular prefilter shader
		
	private:
		//HdrCubeMap's cube
		Object* m_CubeObj = nullptr;
	
		bool						m_IsInitialize = false;

		//Quad Object is used to store brdf look up table (brdfLUTMap)
		Object*						m_LUTQuad = nullptr;

		glm::mat4					m_CaptureProjection;
		std::vector<glm::mat4>		m_CaptureViews;
		std::vector<glm::mat4>		m_CaptureProjectionViews;
		unsigned int				m_HdrCubeMapID;
		unsigned int				m_IrradianceCubeMapID;
		unsigned int				m_PrefilterCubeMapID;
		unsigned int				m_BRDFLUTTextureID;

		
	};

}