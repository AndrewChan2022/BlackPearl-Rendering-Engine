#include "pch.h"
#include "BasicRenderer.h"
#include "BlackPearl/Component/LightComponent/LightSources.h"
#include "BlackPearl/Component/LightComponent/ParallelLight.h"
#include "BlackPearl/Component/LightComponent/PointLight.h"
#include "BlackPearl/Component/LightComponent/SpotLight.h"

#include "BlackPearl/Component/MeshRendererComponent/MeshRenderer.h"
#include "BlackPearl/Component/TransformComponent/Transform.h"
#include "BlackPearl/Renderer/Shader/Shader.h"
#include "BlackPearl/Renderer/Renderer.h"
#include "BlackPearl/Renderer/MasterRenderer/ShadowMapPointLightRenderer.h"
namespace BlackPearl {

	BasicRenderer::BasicRenderer()
	{
	}

	BasicRenderer::~BasicRenderer()
	{
	}

	void BasicRenderer::RenderScene(const std::vector<Object*> &objs, const LightSources* lightSources, Renderer::SceneData* scene)
	{
		DrawObjects(objs, scene);
		DrawLightSources(lightSources, scene);

	}

	void BasicRenderer::RenderConfigure(Object * obj)
	{
		//if (obj->HasComponent<MeshRenderer>()) {
		//	auto meshRenderer = obj->GetComponent<MeshRenderer>();
		//	if (meshRenderer->GetEnableCullFace()) {
		//		//glEnable(GL_CULL_FACE);
		//	}
		//	else {
		//		//glDisable(GL_CULL_FACE);

		//	}
		//}
	}

	void BasicRenderer::DrawObjectsExcept(const std::vector<Object*>& objs, const std::vector<Object*>& exceptObjs, Renderer::SceneData* scene)
	{

		for (auto Obj : objs) {
			bool canDraw = true;
			for (auto obj : exceptObjs) {
				if (Obj->GetId().id == obj->GetId().id) {
					canDraw = false;
				}
			}
			if (canDraw)
				DrawObject(Obj, scene);

		}
	}
	void BasicRenderer::DrawObjectsExcept(const std::vector<Object*>& objs, const Object * exceptObj, Renderer::SceneData* scene)
	{

		for (auto obj : objs) {
			if (obj->GetId().id != exceptObj->GetId().id) {
				DrawObject(obj, scene);
			}
		}

	}
	//objs�п��԰���Light
	void BasicRenderer::DrawObjects(std::vector<Object *>objs, Renderer::SceneData* scene)
	{
		for (auto obj : objs) {
			DrawObject(obj, scene);
		}

	}
	void BasicRenderer::DrawObjects(std::vector<Object*> objs, std::shared_ptr<Shader> shader, Renderer::SceneData* scene)
	{
		for (auto obj : objs) {
			DrawObject(obj, shader, scene);
		}
	}
	//ÿ��Meshһ��shader
	void BasicRenderer::DrawObject(Object * obj, Renderer::SceneData* scene)
	{
		GE_ASSERT(obj, "obj is empty!");
		if (!obj->HasComponent<MeshRenderer>() || !obj->GetComponent<MeshRenderer>()->GetEnableRender())
			return;

		glm::mat4 transformMatrix = obj->GetComponent<Transform>()->GetTransformMatrix();
		std::vector<Mesh>& meshes = obj->GetComponent<MeshRenderer>()->GetMeshes();

		//RenderConfigure(obj);

		for (int i = 0; i < meshes.size(); i++) {
			std::shared_ptr<Shader> shader = meshes[i].GetMaterial()->GetShader();
			shader->Bind();
			if (obj->HasComponent<PointLight>() || obj->HasComponent<ParallelLight>() || obj->HasComponent<SpotLight>()) {
				PrepareShaderParameters(meshes[i], transformMatrix, shader, true);
			}
			else
				PrepareShaderParameters(meshes[i], transformMatrix, shader);

			Renderer::Submit(meshes[i].GetVertexArray(), shader, transformMatrix, scene);
			if (meshes[i].GetIndicesSize() > 0) {
				unsigned int indicesNum = meshes[i].GetIndicesSize() / sizeof(unsigned int);
				meshes[i].GetVertexArray()->GetIndexBuffer()->Bind();
				glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_INT, 0);//0
				meshes[i].GetVertexArray()->GetIndexBuffer()->UnBind();

			}
			else 
			{
				meshes[i].GetVertexArray()->UpdateVertexBuffer();
				for (int j = 0; j < meshes[i].GetVertexArray()->GetVertexBuffers().size(); j++)
				{
					auto vertexBuffer = meshes[i].GetVertexArray()->GetVertexBuffers()[j];
					//vertexBuffer->Bind();
					unsigned int vertexNum = vertexBuffer->GetVertexSize() / vertexBuffer->GetBufferLayout().GetStride();
					glDrawArrays(GL_TRIANGLES, 0, vertexNum);
					for (int index = 0; index < vertexBuffer->GetBufferLayout().GetElements().size(); index++)
					{
						glDisableVertexAttribArray(vertexBuffer->GetBufferLayout().GetElements()[index].Location);
					}
					vertexBuffer->UnBind();
				}

			}
			meshes[i].GetVertexArray()->UnBind();
			meshes[i].GetMaterial()->Unbind();


		}
	}
	void BasicRenderer::DrawObject(Object * obj, std::shared_ptr<Shader> shader, Renderer::SceneData* scene)
	{
		GE_ASSERT(obj, "obj is empty!");
		if (!obj->HasComponent<MeshRenderer>() || !obj->GetComponent<MeshRenderer>()->GetEnableRender())
			return;

		glm::mat4 transformMatrix = obj->GetComponent<Transform>()->GetTransformMatrix();
		std::vector<Mesh> meshes = obj->GetComponent<MeshRenderer>()->GetMeshes();

		//RenderConfigure(obj);


		for (int i = 0; i < meshes.size(); i++) {
			if (obj->HasComponent<PointLight>() || obj->HasComponent<ParallelLight>() || obj->HasComponent<SpotLight>()) 
				PrepareShaderParameters(meshes[i], transformMatrix, shader, true);
			else
				PrepareShaderParameters(meshes[i], transformMatrix, shader);

			Renderer::Submit(meshes[i].GetVertexArray(), shader, transformMatrix, scene);
			if (meshes[i].GetIndicesSize() > 0) {
				unsigned int indicesNum = meshes[i].GetIndicesSize() / sizeof(unsigned int);
				meshes[i].GetVertexArray()->GetIndexBuffer()->Bind();
				glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_INT, 0);
				meshes[i].GetVertexArray()->GetIndexBuffer()->UnBind();


			}
			else 
			{
				meshes[i].GetVertexArray()->UpdateVertexBuffer();
				for (int j = 0; j < meshes[i].GetVertexArray()->GetVertexBuffers().size(); j++)
				{
					auto vertexBuffer = meshes[i].GetVertexArray()->GetVertexBuffers()[j];
					unsigned int vertexNum = vertexBuffer->GetVertexSize() / vertexBuffer->GetBufferLayout().GetStride();
					glDrawArrays(GL_TRIANGLES, 0, vertexNum);
					
					for (int index = 0; index < vertexBuffer->GetBufferLayout().GetElements().size(); index++)
					{
						glDisableVertexAttribArray(vertexBuffer->GetBufferLayout().GetElements()[index].Location);
					}
					vertexBuffer->UnBind();
				}
				//glDrawArrays(GL_TRIANGLES, 0, meshes[i].GetVerticesSize() / meshes[i].GetVertexBufferLayout().GetStride());

			}
			meshes[i].GetVertexArray()->UnBind();
			meshes[i].GetMaterial()->Unbind();

		}
	}
	void BasicRenderer::DrawPointLight(Object * obj, Renderer::SceneData* scene)
	{
		GE_ASSERT(obj->HasComponent<PointLight>(), "obj has no pointlight component!");
		glm::mat4 transformMatrix = obj->GetComponent<Transform>()->GetTransformMatrix();
		std::vector<Mesh> meshes = obj->GetComponent<MeshRenderer>()->GetMeshes();


		for (int i = 0; i < meshes.size(); i++) {
			std::shared_ptr<Shader> shader = meshes[i].GetMaterial()->GetShader();
			shader->Bind();
			PrepareShaderParameters(meshes[i], transformMatrix, shader, true);
			Renderer::Submit(meshes[i].GetVertexArray(), shader, transformMatrix, scene);
			if (meshes[i].GetIndicesSize() > 0) {
				unsigned int indicesNum = meshes[i].GetIndicesSize() / sizeof(unsigned int);
				meshes[i].GetVertexArray()->GetIndexBuffer()->Bind();
				glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_INT, 0);
				meshes[i].GetVertexArray()->GetIndexBuffer()->UnBind();

			}
			else {
				meshes[i].GetVertexArray()->UpdateVertexBuffer();

				for (int j = 0; j < meshes[i].GetVertexArray()->GetVertexBuffers().size(); j++)
				{
					auto vertexBuffer = meshes[i].GetVertexArray()->GetVertexBuffers()[j];
					glDrawArrays(GL_TRIANGLES, 0, vertexBuffer->GetVertexSize() / vertexBuffer->GetBufferLayout().GetStride());

					for (int index = 0; index < vertexBuffer->GetBufferLayout().GetElements().size(); index++)
					{
						glDisableVertexAttribArray(vertexBuffer->GetBufferLayout().GetElements()[index].Location);
					}
					vertexBuffer->UnBind();
				}
				//glDrawArrays(GL_TRIANGLES, 0, meshes[i].GetVerticesSize() / meshes[i].GetVertexBufferLayout().GetStride());

			}
			meshes[i].GetVertexArray()->UnBind();

			meshes[i].GetMaterial()->Unbind();
			
		}

	}
	void BasicRenderer::DrawLightSources(const LightSources * lightSources, Renderer::SceneData* scene)
	{
		for (auto lightObj : lightSources->Get()) {
			if (lightObj->HasComponent<PointLight>())
				DrawPointLight(lightObj, scene);
		}
	}
	void BasicRenderer::PrepareShaderParameters(Mesh mesh, glm::mat4 transformMatrix, std::shared_ptr<Shader> shader, bool isLight)
	{
		PrepareBasicShaderParameters(mesh, transformMatrix, shader, isLight);
	}
	void BasicRenderer::PrepareBasicShaderParameters(Mesh mesh, glm::mat4 transformMatrix, std::shared_ptr<Shader> shader, bool isLight)
	{	//TODO::shader����ȾǰBind()!�����⣡
		//������Ҫshader->Bind()�ģ���ʾʹ�����shader!
		//shader->Bind();

		std::shared_ptr<Material> material = mesh.GetMaterial();
		std::shared_ptr<Material::TextureMaps> textures = material->GetTextureMaps();

		//k��4��ʼ��0��1��2��3��texture�����Զ���texture
		bool diffuseMap = false, specularMap = false, emissionMap = false, normalMap = false,mentallicMap=false;
		unsigned int k = 0;
		if (textures != nullptr) {
			if (textures->diffuseTextureMap != nullptr) {
				diffuseMap = true;
				glActiveTexture(GL_TEXTURE0 + k);
				shader->SetUniform1i("u_Material.diffuse", k);
				shader->SetUniform1i("u_Material.isDiffuseTextureSample", 1);

				textures->diffuseTextureMap->Bind();
				k++;
			}
			if (textures->specularTextureMap != nullptr) {
				specularMap = true;
				glActiveTexture(GL_TEXTURE0 + k);
				shader->SetUniform1i("u_Material.specular", k);
				shader->SetUniform1i("u_Material.isSpecularTextureSample", 1);

				textures->specularTextureMap->Bind();
				k++;
			}
			if (textures->emissionTextureMap != nullptr) {
				emissionMap = true;
				glActiveTexture(GL_TEXTURE0 + k);

				shader->SetUniform1i("u_Material.emission", k);
				textures->emissionTextureMap->Bind();
				k++;
			}
			if (textures->heightTextureMap != nullptr) {
				glActiveTexture(GL_TEXTURE0 + k);

				shader->SetUniform1i("u_Material.height", k);
				textures->heightTextureMap->Bind();
				k++;
			}
			if (textures->normalTextureMap != nullptr) {
				normalMap = true;
				glActiveTexture(GL_TEXTURE0 + k);

				shader->SetUniform1i("u_Material.normal", k);
				textures->normalTextureMap->Bind();
				k++;
			}
			if (textures->cubeTextureMap != nullptr) {
				glActiveTexture(GL_TEXTURE0 + k);
				shader->SetUniform1i("u_Material.cube", k);
				//shader->SetUniform1i("u_cubeMap", k);
				textures->cubeTextureMap->Bind();
				k++;
			}
			if (textures->depthTextureMap != nullptr) {
				glActiveTexture(GL_TEXTURE0 + k);
				shader->SetUniform1i("u_Material.depth", k);

				textures->depthTextureMap->Bind();
				k++;
			}
			if (textures->aoMap != nullptr) {
				glActiveTexture(GL_TEXTURE0 + k);
				shader->SetUniform1i("u_Material.ao", k);

				textures->aoMap->Bind();
				k++;
			}
			if (textures->roughnessMap != nullptr) {
				glActiveTexture(GL_TEXTURE0 + k);
				shader->SetUniform1i("u_Material.roughness", k);

				textures->roughnessMap->Bind();
				k++;
			}
			if (textures->mentallicMap != nullptr) {
				mentallicMap = true;
				glActiveTexture(GL_TEXTURE0 + k);
				shader->SetUniform1i("u_Material.mentallic", k);
				shader->SetUniform1i("u_Material.isMetallicTextureSample", 1);


				textures->mentallicMap->Bind();
				k++;
			}
		}
		MaterialColor::Color materialColor = mesh.GetMaterial()->GetMaterialColor().Get();
		if (diffuseMap==false) {
			shader->SetUniformVec3f("u_Material.diffuseColor", materialColor.diffuseColor);
			shader->SetUniform1i("u_Material.isDiffuseTextureSample", 0);

		}
		if (specularMap == false) {
			shader->SetUniformVec3f("u_Material.specularColor", materialColor.specularColor);
			shader->SetUniform1i("u_Material.isSpecularTextureSample", 0);

		}
		if (mentallicMap == false){
			shader->SetUniformVec3f("u_Material.mentallicValue", materialColor.specularColor);
			shader->SetUniform1i("u_Material.isMetallicTextureSample", 0);

		}
		shader->SetUniformVec3f("u_Material.ambientColor", materialColor.ambientColor);
		shader->SetUniformVec3f("u_Material.emissionColor", materialColor.emissionColor);

		shader->SetUniformMat4f("u_TranInverseModel", glm::transpose(glm::inverse(transformMatrix)));
		shader->SetUniform1f("u_Material.shininess", material->GetProps().shininess);
		shader->SetUniform1i("u_Material.isBlinnLight", material->GetProps().isBinnLight);
		shader->SetUniform1i("u_Material.isTextureSample", material->GetProps().isTextureSample);

		if (!material->GetProps().isTextureSample) {

			shader->SetUniform1f("u_Material.roughnessValue", 1.0f);
			shader->SetUniform1f("u_Material.mentallicValue", 0.0f);
			shader->SetUniform1f("u_Material.aoValue", 1.0f);


		}


		shader->SetExtraUniform();
		if (!isLight) {
			shader->SetUniform1i("u_Settings.shadows", 1);

			shader->SetLightUniform(Renderer::GetSceneData()->LightSources);
		}
		else {
			shader->SetUniform1i("u_Settings.shadows", 0);

		}
	}

}