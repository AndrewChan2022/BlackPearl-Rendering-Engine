#include "pch.h"
#include "ShadowMapPointLightRenderer.h"
#include "BlackPearl/Component/LightComponent/PointLight.h"
#include "BlackPearl/Component/MeshRendererComponent/MeshRenderer.h"
#include "BlackPearl/Component/TransformComponent/Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace BlackPearl {
	int ShadowMapPointLightRenderer::s_ShadowMapPointLightWidth =1024;
	int ShadowMapPointLightRenderer::s_ShadowMapPointLightHeight =  1024;
	float ShadowMapPointLightRenderer::s_FarPlane = 60.0f;
	float ShadowMapPointLightRenderer::s_FOV = 90.0f;
	//NearPlane ���ɸı��뱣֤��1 ��shader��Ҫת����[0,1]����ϵ
	//CubeMapDepthShader.glsl
	const float ShadowMapPointLightRenderer::s_NearPlane = 1.0f;

	ShadowMapPointLightRenderer::ShadowMapPointLightRenderer()
	{

		/*m_FrameBuffer.reset(DBG_NEW FrameBuffer(s_ShadowMapPointLightWidth, s_ShadowMapPointLightHeight,
			{ FrameBuffer::Attachment::CubeMapDepthTexture }, 0, true));*/

		m_FrameBuffer.reset(DBG_NEW FrameBuffer());
		//m_DepthCubeMap.reset(DBG_NEW CubeMapTexture(Texture::Type::CubeMap, s_ShadowMapPointLightWidth, s_ShadowMapPointLightWidth, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT));
		

		m_LightProjectionViewMatries.assign(6, glm::mat4(1.0));
		m_SimpleDepthShader.reset(DBG_NEW BlackPearl::Shader("assets/shaders/shadowMap/pointLight/CubeMapDepthShader.glsl"));
		m_ShadowMapShader.reset(DBG_NEW BlackPearl::Shader("assets/shaders/shadowMap/pointLight/CubeMapShadowMapping.glsl"));


	}

	ShadowMapPointLightRenderer::~ShadowMapPointLightRenderer()
	{
	}
	void ShadowMapPointLightRenderer::RenderCubeMap(const std::vector<Object*>& objs, LightSources* lightSources)
	{
		float aspect = (float)s_ShadowMapPointLightWidth / (float)s_ShadowMapPointLightHeight;
		glm::mat4 pointLightProjection = glm::perspective(glm::radians(s_FOV), aspect, s_NearPlane, s_FarPlane);

			//TODO:���PointLights
		GE_ASSERT(lightSources->GetPointLights().size() >0, "no pointlight found!");

		for (Object* pointLight:lightSources->GetPointLights())
		{
			std::shared_ptr<CubeMapTexture> shadowMap = pointLight->GetComponent<PointLight>()->GetShadowMap();
			
			m_FrameBuffer->Bind();
			m_FrameBuffer->AttachCubeMapDepthTexture(shadowMap);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			m_FrameBuffer->UnBind();
			glm::vec3 lightPos = pointLight->GetComponent<Transform>()->GetPosition();
			m_LightProjectionViewMatries[0] = (pointLightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0, -1.0f, 0.0)));
			m_LightProjectionViewMatries[1] = (pointLightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0, -1.0f, 0.0)));
			m_LightProjectionViewMatries[2] = (pointLightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0f, 0.0), glm::vec3(0.0, 0.0, 1.0f)));
			m_LightProjectionViewMatries[3] = (pointLightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0f, 0.0), glm::vec3(0.0, 0.0, -1.0f)));
			m_LightProjectionViewMatries[4] = (pointLightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0f), glm::vec3(0.0, -1.0f, 0.0)));
			m_LightProjectionViewMatries[5] = (pointLightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0f), glm::vec3(0.0, -1.0f, 0.0)));


			glViewport(0, 0, shadowMap->GetWidth(), shadowMap->GetHeight());
			m_FrameBuffer->Bind();
			glClear(GL_DEPTH_BUFFER_BIT);


			for (Object* obj : objs)
			{
				m_SimpleDepthShader->Bind();

				for (int i = 0; i < 6; i++)
					m_SimpleDepthShader->SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", m_LightProjectionViewMatries[i]);
				m_SimpleDepthShader->SetUniform1f("u_FarPlane", s_FarPlane);
				m_SimpleDepthShader->SetUniformVec3f("u_LightPos", lightPos);

				obj->GetComponent<MeshRenderer>()->SetShaders(m_ShadowMapShader);
				DrawObject(obj, m_SimpleDepthShader);

			}

			m_FrameBuffer->UnBind();




		}
		



		//m_LightPos = lightSources->GetPointLights()[0]->GetComponent<Transform>()->GetPosition();

	
		
		

	}
	void ShadowMapPointLightRenderer::Render(const std::vector<Object*> backgroundObjs, LightSources* lightSources)
	{
		// 2. Render scene as normal 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		/*glDisable(GL_CULL_FACE);
		m_Shader->SetUniformVec3f("u_LightPos", m_PointLight->GetComponent<BlackPearl::Transform>()->GetPosition());

		m_Shader->SetUniform1i("u_ReverseNormals", 1);
		m_BacisRender->DrawObject(Room);
		m_Shader->SetUniform1i("u_ReverseNormals", 0);

		glEnable(GL_CULL_FACE);
		m_Shader->Bind();


		m_Shader->SetUniform1f("u_FarPlane", BlackPearl::ShadowMapPointLightRenderer::s_FarPlane);*/

		for (Object* obj : backgroundObjs) {
			m_ShadowMapShader->Bind();
			GE_ASSERT(lightSources->GetPointLights().size() >= 1, "no pointlight found!");
		//	m_ShadowMapShader->SetUniformVec3f("u_LightPos", lightSources->GetPointLights()[0]->GetComponent<Transform>()->GetPosition());
			m_ShadowMapShader->SetUniform1f("u_FarPlane", s_FarPlane);

			unsigned int textureId = 0;
			for (Object* pointLight:lightSources->GetPointLights())
			{
				m_ShadowMapShader->SetUniform1i("u_ShadowMap["+std::to_string(textureId)+"]", textureId);
				glActiveTexture(GL_TEXTURE0+ textureId);
				pointLight->GetComponent<PointLight>()->GetShadowMap()->Bind();
				textureId++;
			}

			
			DrawObject(obj, m_ShadowMapShader);
		}
		
		DrawLightSources(lightSources);



	}
	
}

