#include "pch.h"
#include "Model.h"
#include "GameEngine/Renderer/Texture/Texture.h"
//#include "GameEngine/Renderer/Renderer.h"
static void glmInsertVector(glm::vec2 v, std::vector<float> &vec) {
	vec.push_back(v.x);
	vec.push_back(v.y);

}
static void glmInsertVector(glm::vec3 v, std::vector<float> &vec) {
	vec.push_back(v.x);
	vec.push_back(v.y);
	vec.push_back(v.z);
}

void Model::Draw(const std::shared_ptr<Shader>& shader, const glm::mat4 & model, const LightSources& lightSources)
{
	for (unsigned int i = 0; i < m_Meshes.size(); i++)
	{
		m_Meshes[i].Draw(shader, model, lightSources);

	}

}




void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path,
		aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_ValidateDataStructure | 0);
	///*
	//�趨aiProcess_Triangulate�����Ǹ���Assimp�����ģ�Ͳ��ǣ�ȫ��������������ɣ�
	//����Ҫ��ģ�����е�ͼԪ��״�任Ϊ�����Ρ�
	//
	//aiProcess_FlipUVs���ڴ����ʱ��תy����������꣨����ܻ��ǵ�����������̳���˵����
	//��OpenGL�д󲿷ֵ�ͼ���y�ᶼ�Ƿ��ģ�����������ڴ���ѡ����޸������
	//
	//*/
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		GE_CORE_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString())
			return;
	}
	m_Directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh * aimesh, const aiScene * scene)
{
	//float * vertices;
	//uint32_t verticesSize;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	// maps
	std::vector<std::shared_ptr<Texture >> textures;

	VertexBufferLayout layout = {
		{ElementDataType::Float3,"aPos",false},
		{ElementDataType::Float3,"aNormal",false},
		{ElementDataType::Float2,"aTexCoord",false}
	//	{ElementDataType::Float3,"aTangent",false},
	//	{ElementDataType::Float3,"aBitTangent",false},

	};

	for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
	{
		glm::vec3 pos;
		pos.x = aimesh->mVertices[i].x;
		pos.y = aimesh->mVertices[i].y;
		pos.z = aimesh->mVertices[i].z;
		glmInsertVector(pos, vertices);

		glm::vec3 normal;
		normal.x = aimesh->mNormals[i].x;
		normal.y = aimesh->mNormals[i].y;
		normal.z = aimesh->mNormals[i].z;
		glmInsertVector(normal, vertices);

		glm::vec2 textCords = glm::vec2(0.0f, 0.0f);
		if (aimesh->mTextureCoords[0]) {//�ж϶����Ƿ��в�������

			textCords.x = aimesh->mTextureCoords[0][i].x;
			textCords.y = aimesh->mTextureCoords[0][i].y;
		}
		glmInsertVector(textCords, vertices);


		//tangent
		//glm::vec3 tangent = glm::vec3(0.0f);
		//if (aimesh->mTangents != NULL) {
		//	tangent.x = aimesh->mTangents[i].x;
		//	tangent.y = aimesh->mTangents[i].y;
		//	tangent.z = aimesh->mTangents[i].z;

		//}
		//glmInsertVector(tangent, vertices);

		////bittangent
		//glm::vec3 bitTangent = glm::vec3(0.0f);
		//if (aimesh->mBitangents != NULL) {
		//	bitTangent.x = aimesh->mBitangents[i].x;
		//	bitTangent.y = aimesh->mBitangents[i].y;
		//	bitTangent.z = aimesh->mBitangents[i].z;
		//}

		//glmInsertVector(bitTangent, vertices);

	}


	for (unsigned int i = 0; i < aimesh->mNumFaces; i++)
	{
		aiFace face = aimesh->mFaces[i];

		for (unsigned int i = 0; i < face.mNumIndices; i++)
		{
			indices.push_back(face.mIndices[i]);

		}
	}
	if (aimesh->mMaterialIndex >=0) {
		aiMaterial *material = scene->mMaterials[aimesh->mMaterialIndex];

		LoadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Type::DiffuseMap, textures);
		LoadMaterialTextures(material, aiTextureType_SPECULAR, Texture::Type::SpecularMap, textures);
		LoadMaterialTextures(material, aiTextureType_NORMALS, Texture::Type::NormalMap, textures);
		LoadMaterialTextures(material, aiTextureType_HEIGHT, Texture::Type::HeightMap, textures);

	}



	float * vertices_ = new float[vertices.size()];
	//if (vertices.size() > 0)
		memcpy(vertices_, &vertices[0], vertices.size()*sizeof(float));//ע��memcpy���һ���������ֽ���!!!

	unsigned int* indices_ = new unsigned int[indices.size()];
	//if (indices.size() > 0)
		memcpy(indices_, &indices[0], indices.size()*sizeof(unsigned int));



	return Mesh( vertices_, vertices.size()*sizeof(float),indices_, indices.size()*sizeof(unsigned int), textures, layout);

}

void Model::LoadMaterialTextures(aiMaterial * material, aiTextureType type, Texture::Type typeName, std::vector<std::shared_ptr<Texture >> &textures)
{

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		std::shared_ptr<Texture> texture;
		aiString path;
		material->GetTexture(type, i, &path);

		std::string path_str = m_Directory + "/" + std::string(path.C_Str());
		texture.reset(new Texture(typeName, path_str.c_str()));
		textures.push_back(texture);
	}

}
