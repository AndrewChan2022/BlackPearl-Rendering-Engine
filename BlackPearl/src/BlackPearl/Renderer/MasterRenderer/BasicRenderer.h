#pragma once
#include "BlackPearl/Component/LightComponent/LightSources.h"
#include "BlackPearl/Renderer/Mesh/Mesh.h"
#include "BlackPearl/Renderer/Renderer.h"

namespace BlackPearl {
	class BasicRenderer
	{
	public:
		BasicRenderer();
		~BasicRenderer();
		void RenderScene(const std::vector<Object *>&objs, const LightSources* lightSources,Renderer::SceneData *scene = Renderer::GetSceneData());
		
		//ÿ��Object��renderǰ������
		void RenderConfigure(Object * obj);
		//TODO::��������������ɾ��
		void DrawObjectsExcept(const std::vector<Object*>& objs, const std::vector<Object*>& exceptObjs, Renderer::SceneData* scene = Renderer::GetSceneData());
		void DrawObjectsExcept(const std::vector<Object*>& objs, const Object* exceptObj, Renderer::SceneData* scene = Renderer::GetSceneData());

		//��ͬobjectʹ�ò�ͬshader,ʹ��ǰҪ������shader
		void DrawObjects(std::vector<Object *>objs, Renderer::SceneData* scene = Renderer::GetSceneData());
		//����Objectsʹ��ͬһ��shader
		void DrawObjects(std::vector<Object *>objs,std::shared_ptr<Shader> shader, Renderer::SceneData* scene = Renderer::GetSceneData(), unsigned int textureBeginIdx = 2);
		//ʹ��ǰҪ������shader,���Object�Ĳ�ͬMesh�����в�ͬ��shader
		void DrawObject(Object *obj, Renderer::SceneData* scene = Renderer::GetSceneData(),unsigned int textureBeginIdx=2);
		//���Object������Meshʹ��ͬһ��shader
		void DrawObject(Object *obj, std::shared_ptr<Shader> shader, Renderer::SceneData* scene = Renderer::GetSceneData(), unsigned int textureBeginIdx = 2);

		void DrawPointLight(Object *obj, Renderer::SceneData* scene = Renderer::GetSceneData(), unsigned int textureBeginIdx = 2);
		void DrawLightSources(const LightSources* lightSources, Renderer::SceneData* scene = Renderer::GetSceneData(), unsigned int textureBeginIdx = 2);
		void PrepareBasicShaderParameters(Mesh mesh,std::shared_ptr<Shader> shader, bool isLight = false, unsigned int textureBeginIdx = 2);

		//virtual void PrepareShaderParameters(Mesh mesh, std::shared_ptr<Shader> shader, bool isLight = false);

	};


}
