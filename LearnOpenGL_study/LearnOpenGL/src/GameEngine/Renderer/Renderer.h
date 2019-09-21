#pragma once
#include"GameEngine/Renderer/VertexArray.h"
#include "GameEngine/Renderer/Shader.h"
#include "GameEngine/Renderer/Camera/Camera.h"
class Renderer
{
public:
	struct SceneData {
		glm::mat4 ViewProjectionMatrix;
		glm::vec3 CameraPosition;
		glm::vec3 CameraFront;
	};

	Renderer();
	~Renderer();
	static void Init();
	static void BeginScene(const Camera& camera, const LightSources& lightSources);//ÿ��Update��Ҫ����BeginSceneһ�Σ���ΪCamera��ViewProjection Matrix��ı�
	static void Submit(const std::shared_ptr<VertexArray>& vertexArray,const std::shared_ptr<Shader>& shader,const glm::mat4 &model = glm::mat4(1.0f));//Submmitǰ�ǵõ��� BeginScene()!
	static SceneData* GetSceneData() { return m_SceneData; }
private:
	
	static SceneData* m_SceneData;
};

