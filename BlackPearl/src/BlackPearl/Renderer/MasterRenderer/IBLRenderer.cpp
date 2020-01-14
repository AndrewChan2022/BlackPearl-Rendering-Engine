#include "pch.h"
#include "IBLRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "BlackPearl/Component/MeshRendererComponent/MeshRenderer.h"
#include "BlackPearl/Component/TransformComponent/Transform.h"
namespace BlackPearl {
	int IBLRenderer::s_Width = 512;
	int IBLRenderer::s_Height = 512;
	IBLRenderer::IBLRenderer()
	{

		m_FrameBuffer.reset(DBG_NEW FrameBuffer(s_Width, s_Height,
			{ FrameBuffer::Attachment::CubeMapColorTexture,FrameBuffer::Attachment::RenderBuffer }, 0, false));

		//Load Hdr environment map
		m_HdrMapToCubeShader.reset(DBG_NEW Shader("assets/shaders/ibl/hdrMapToCube.glsl"));
		m_IBLShader.reset(DBG_NEW Shader("assets/shaders/ibl/ibl.glsl"));
		//m_HdrTexture.reset(DBG_NEW HDRTexture("assets/texture/hdrEnvironmentMap/sIBL-Collection/sIBL_Collection/backlot/Diffuse10.hdr")); 
		m_HdrTexture.reset(DBG_NEW HDRTexture("assets/texture/hdrEnvironmentMap/sIBL-Collection/sIBL_Collection/OfficeEden/Eden_REF.hdr")); :																																  //m_HdrTexture.reset(DBG_NEW HDRTexture("assets/texture/hdrEnvironmentMap/sIBL-Collection/sIBL_Collection/apartment/Apartment_Reflection.hdr")); //TODO::
		//m_HdrTexture.reset(DBG_NEW HDRTexture("assets/texture/hdrEnvironmentMap/Desert_Highway/Road_to_MonumentValley_Ref.hdr")); 
		m_IrradianceShader.reset(DBG_NEW Shader("assets/shaders/ibl/irradianceConvolution.glsl"));

		//specular shader
		m_PrefilterShader.reset(DBG_NEW Shader("assets/shaders/ibl/prefilterMap.glsl"));
		m_BRDFShader.reset(DBG_NEW Shader("assets/shaders/ibl/brdf.glsl"));

	}
	void IBLRenderer::Init(Object* hdrCubeObj, Object* brdfLUTQuad)
	{
		GE_ASSERT(hdrCubeObj, "hdrCubeObj is nullptr!");
		GE_ASSERT(brdfLUTQuad, "brdfLUTQuad is nullptr!");

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		m_CubeObj = hdrCubeObj;
		m_LUTQuad = brdfLUTQuad;
		RenderHdrMapToEnvironmentCubeMap(); //for environment skybox
		RenderIrradianceMap(); //for diffuse GI
		RenderPrefilterMap(); //for specular GI's irradiance part
		RenderBRDFLUTMap(); //for specular GI's brdf part
		m_IsInitialize = true;
	}
	IBLRenderer::~IBLRenderer()
	{
	}
	void IBLRenderer::RenderHdrMapToEnvironmentCubeMap() {


		float aspect = (float)s_Width / (float)s_Height;
		m_CaptureProjection = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 10.0f);
		m_CaptureProjectionViews =
		{
			m_CaptureProjection * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			m_CaptureProjection * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			m_CaptureProjection * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			m_CaptureProjection * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			m_CaptureProjection * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			m_CaptureProjection * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		m_HdrMapToCubeShader->Bind();
		glActiveTexture(GL_TEXTURE0 + 1);
		m_HdrTexture->Bind();
		m_HdrMapToCubeShader->SetUniform1i("hdrTexture", 1);

		//Draw CubeMap from hdrMap
		glViewport(0, 0, s_Width, s_Height);
		m_FrameBuffer->Bind();

		/*
		��������������������������ͬ����ͼ����
		����ͶӰ����� fov Ϊ 90 ���Բ�׽�����棬
		����Ⱦ���������Σ�������洢�ڸ���֡������
		*/
		for (unsigned int i = 0; i < 6; i++)
		{
			m_HdrMapToCubeShader->SetUniformMat4f("u_CubeMapProjectionView", m_CaptureProjectionViews[i]);
			//��CubeMap�������渽�ӵ�FrameBuffer��GL_COLOR_ATTACHMENT0��
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_FrameBuffer->GetCubeMapColorTexture(0)->GetRendererID(), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_CubeObj->GetComponent<MeshRenderer>()->SetShaders(m_HdrMapToCubeShader);
			DrawObject(m_CubeObj, m_HdrMapToCubeShader);
		}
		m_HdrCubeMapID = m_FrameBuffer->GetCubeMapColorTexture(0)->GetRendererID();
		m_FrameBuffer->UnBind();
	}
	void IBLRenderer::RenderIrradianceMap()
	{
		// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_HdrCubeMapID);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glGenTextures(1, &m_IrradianceCubeMapID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceCubeMapID);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		m_FrameBuffer->Bind();
		glBindRenderbuffer(GL_RENDERBUFFER, m_FrameBuffer->GetRenderBufferID());
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);


		m_IrradianceShader->Bind();
		m_IrradianceShader->SetUniform1i("u_EnvironmentMap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_HdrCubeMapID);
		// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
		glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
		m_FrameBuffer->Bind();

		for (unsigned int i = 0; i < 6; ++i)
		{
			m_IrradianceShader->SetUniformMat4f("u_CubeMapProjectionView", m_CaptureProjectionViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceCubeMapID, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			DrawObject(m_CubeObj, m_IrradianceShader);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void IBLRenderer::RenderPrefilterMap()
	{
		glGenTextures(1, &m_PrefilterCubeMapID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterCubeMapID);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
		// ----------------------------------------------------------------------------------------------------
		m_PrefilterShader->Bind();
		m_PrefilterShader->SetUniform1i("u_EnvironmentMap", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_HdrCubeMapID);

		m_FrameBuffer->Bind();
		unsigned int maxMipMapLevels = 5;
		for (unsigned int mip = 0; mip < maxMipMapLevels; mip++)
		{
			//resize framebuffer according to mipmap-level size;
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);
			m_FrameBuffer->BindRenderBuffer();
			//glBindRenderbuffer(GL_RENDERBUFFER, m_FrameBuffer->GetRenderBufferID());
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipMapLevels - 1);
			m_PrefilterShader->SetUniform1f("u_roughness", roughness);
			for (unsigned int i = 0; i < 6; i++)
			{
				m_PrefilterShader->SetUniformMat4f("u_CubeMapProjectionView", m_CaptureProjectionViews[i]);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterCubeMapID, mip);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				m_CubeObj->GetComponent<MeshRenderer>()->SetShaders(m_PrefilterShader);
				DrawObject(m_CubeObj, m_PrefilterShader);

			}


		}


		m_FrameBuffer->UnBind();
	}

	void IBLRenderer::RenderBRDFLUTMap()
	{
		std::shared_ptr<Texture> brdfLUTTexture(new Texture(Texture::DiffuseMap, 512, 512, false, GL_LINEAR, GL_LINEAR, GL_RG16F, GL_RG, GL_CLAMP_TO_EDGE, GL_FLOAT));
		//std::shared_ptr<Texture> brdfLUTTexture(new Texture(Texture::None, 512, 512, GL_LINEAR, GL_LINEAR, GL_RG16F, GL_RG, GL_CLAMP_TO_EDGE, GL_FLOAT));
		m_FrameBuffer->Bind();
		m_FrameBuffer->BindRenderBuffer();
		m_FrameBuffer->AttachColorTexture(brdfLUTTexture, 0);//����attach 0 
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);

		m_BRDFLUTTextureID = brdfLUTTexture->GetRendererID();

		glViewport(0, 0, 512, 512);
		m_BRDFShader->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawObject(m_LUTQuad, m_BRDFShader);
		m_FrameBuffer->UnBind();

	}

	void IBLRenderer::RenderSpheres(Object* sphere)
	{
		int rows = 7;
		int cols = 7;
		int spacing = 2;
		m_IBLShader->Bind();
		m_IBLShader->SetUniform1i("u_IrradianceMap", 0);
		m_IBLShader->SetUniform1i("u_PrefilterMap", 1);
		m_IBLShader->SetUniform1i("u_BrdfLUTMap", 2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceCubeMapID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterCubeMapID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_BRDFLUTTextureID);



		m_IBLShader->SetUniform1f("u_ao", 1.0f);
		for (int row = 0; row < rows; row++) {
			m_IBLShader->SetUniform1f("u_metallic", (float)row / (float)rows);

			for (int col = 0; col < cols; col++) {

				sphere->GetComponent<Transform>()->SetPosition({
					(col - (cols / 2)) * spacing,
					(row - (rows / 2)) * spacing,
					0.0f });
				m_IBLShader->SetUniform1f("u_roughness", glm::clamp((float)col / (float)cols, 0.05f, 1.0f));
				sphere->GetComponent<MeshRenderer>()->SetShaders(m_IBLShader);
				DrawObject(sphere, m_IBLShader);

			}
		}



	}


	void IBLRenderer::DrawBRDFLUTMap()
	{
		glViewport(0, 0, 240, 240);
		DrawObject(m_LUTQuad, m_BRDFShader);
		glViewport(0, 0, BlackPearl::Configuration::WindowWidth, BlackPearl::Configuration::WindowHeight);
	}

	void IBLRenderer::Render(std::vector<Object*> objs)
	{
	}
}