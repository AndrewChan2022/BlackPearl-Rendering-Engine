#pragma once
#include "BlackPearl/Component/LightComponent/LightSources.h"
#include "BlackPearl/Renderer/Mesh/Mesh.h"
namespace BlackPearl {
	class BasicRenderer
	{
	public:
		BasicRenderer();
		~BasicRenderer();
		void RenderScene(const std::vector<Object *>&objs, const LightSources* lightSources);
		
		//ÿ��Object��renderǰ������
		void RenderConfigure(Object * obj);
		//TODO::��������������ɾ��
		void DrawObjectsExcept(const std::vector<Object*>& objs, const std::vector<Object*>& exceptObjs);
		void DrawObjectsExcept(const std::vector<Object*>& objs, const Object* exceptObj);

		//��ͬobjectʹ�ò�ͬshader,ʹ��ǰҪ������shader
		void DrawObjects(std::vector<Object *>objs);
		//����Objectsʹ��ͬһ��shader
		void DrawObjects(std::vector<Object *>objs,std::shared_ptr<Shader> shader);
		//ʹ��ǰҪ������shader,���Object�Ĳ�ͬMesh�����в�ͬ��shader
		void DrawObject(Object *obj);
		//���Object������Meshʹ��ͬһ��shader
		void DrawObject(Object *obj, std::shared_ptr<Shader> shader);

		void DrawPointLight(Object *obj);
		void DrawLightSources(const LightSources* lightSources);
		void PrepareBasicShaderParameters(Mesh &mesh, glm::mat4 transformMatrix,std::shared_ptr<Shader> shader, bool isLight = false);

		virtual void PrepareShaderParameters(Mesh &mesh, glm::mat4 transformMatrix, std::shared_ptr<Shader> shader, bool isLight = false);

	};


}
