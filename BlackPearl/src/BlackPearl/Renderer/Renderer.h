#pragma once
#include"BlackPearl/Renderer/VertexArray.h"
#include "BlackPearl/Renderer/Shader.h"
#include "BlackPearl/Component/CameraComponent/Camera.h"
namespace BlackPearl {

	class Renderer
	{
	public:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::vec3 CameraPosition;
			glm::vec3 CameraFront;
			LightSources LightSources;
		};

		Renderer();
		~Renderer();
		static void Init();
		static void BeginScene(const Camera& camera, const LightSources& lightSources);//ÿ��Update��Ҫ����BeginSceneһ�Σ���ΪCamera��ViewProjection Matrix��ı�
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4 &model = glm::mat4(1.0f));//Submmitǰ�ǵõ��� BeginScene()!
		static SceneData* GetSceneData() { return m_SceneData; }
	private:

		static SceneData* m_SceneData;
	};

}