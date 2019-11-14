#pragma once
#include <string>
#include <vector>



namespace BlackPearl {

	class Texture
	{
	public:
		enum Type {
			DiffuseMap,
			SpecularMap,
			EmissionMap,
			NormalMap,
			HeightMap,
			CubeMap
		};
		Texture(Type type, const std::string &image);
		//û��Image��texture
		Texture(Type type, const int width,const int height);

		//����CubeMap��ʼ��
		Texture(Type type, std::vector<std::string> faces);


		virtual ~Texture() = default;

		virtual void LoadTexture(const std::string &image);
		virtual void Bind();
		virtual void UnBind();
		inline Type GetType() { return m_Type; }
		unsigned int GetRendererID() { return m_TextureID; }


	protected:
		unsigned int m_TextureID;
		std::string m_Path;
		Type m_Type;
	};



}