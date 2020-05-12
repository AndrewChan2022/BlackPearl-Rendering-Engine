#pragma once
#include "Texture.h"
#include "BlackPearl/Renderer/Shader/Shader.h"
#include "MaterialColor.h"
namespace BlackPearl {
	class Material
	{
	public:
		struct Props {
			//Enable texture
			float shininess;
			bool  isBinnLight;
			int  isPBRTextureSample;//�Ƿ�ʹ������-->���� ao,normal,metalllic,roughness
			int  isDiffuseTextureSample;//�Ƿ�ʹ������
			int  isSpecularTextureSample;//�Ƿ�ʹ������
		//	int  isMetallicTextureSample;//�Ƿ�ʹ������
			int  isEmissionTextureSample;//�Ƿ�ʹ������

			Props() :shininess(64.0f), isBinnLight(false), isPBRTextureSample(0),isDiffuseTextureSample(0),isSpecularTextureSample(0),isEmissionTextureSample(0){}

		};
		struct TextureMaps {
			std::shared_ptr<Texture> diffuseTextureMap;
			std::shared_ptr<Texture> specularTextureMap;
			std::shared_ptr<Texture> emissionTextureMap;
			std::shared_ptr<Texture> normalTextureMap;
			std::shared_ptr<Texture> heightTextureMap;
			std::shared_ptr<Texture> cubeTextureMap;
			std::shared_ptr<Texture> depthTextureMap;
			std::shared_ptr<Texture> aoMap;
			std::shared_ptr<Texture> roughnessMap;
			std::shared_ptr<Texture> mentallicMap;

			TextureMaps() {
				diffuseTextureMap
					= specularTextureMap
					= emissionTextureMap
					= normalTextureMap
					= heightTextureMap
					= cubeTextureMap
					= depthTextureMap
					= aoMap
					= roughnessMap
					= mentallicMap
					= nullptr;
			}
		};

		Material(
			const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<TextureMaps>& textureMaps,
			const MaterialColor& materialColors
		)
			:m_Shader(shader), m_TextureMaps(textureMaps), m_MaterialColors(materialColors), m_Props(Props()) {}

		Material(
			const std::string shaderPath,
			const std::shared_ptr<TextureMaps>& textureMaps,
			glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 emissionColor
		);

		~Material();

		std::shared_ptr<Shader>      GetShader()const { return m_Shader; }
		std::shared_ptr<TextureMaps> GetTextureMaps()const { return m_TextureMaps; }
		MaterialColor                GetMaterialColor()const { return m_MaterialColors; }
		Props                        GetProps() const { return m_Props; }

		void SetShader(const std::string& shaderPath);
		void SetShader(const std::shared_ptr<Shader>& shader);
		void SetTexture(const std::shared_ptr<Texture> texture);
		void SetTexture(const Texture::Type type, const std::string& image);
		void SetMaterialColor(MaterialColor::Color color);
		void SetMaterialColorDiffuseColor(glm::vec3 color);
		void SetMaterialColorSpecularColor(glm::vec3 color);
		void SetMaterialColorEmissionColor(glm::vec3 color);


		void SetProps(const Props& props);
		void SetShininess(float shininess);
		void SetBinnLight(bool isBinnLight);
		void SetPBRTextureSample(int isPBRTextureSample);
		void SetTextureSampleDiffuse(int isTextureSampleDiffuse);
		void SetTextureSampleSpecular(int isTextureSampleSpecular);
		//void SetTextureSampleMetallic(int isTextureSampleMetallic);
		void SetTextureSampleEmission(int isTextureSampleMetallic);

		void Unbind() {
			if (m_TextureMaps->diffuseTextureMap != nullptr)   m_TextureMaps->diffuseTextureMap->UnBind();
			if (m_TextureMaps->specularTextureMap != nullptr)	  m_TextureMaps->specularTextureMap->UnBind();
			if (m_TextureMaps->emissionTextureMap != nullptr)	  m_TextureMaps->emissionTextureMap->UnBind();
			if (m_TextureMaps->normalTextureMap != nullptr)	  m_TextureMaps->normalTextureMap->UnBind();
			if (m_TextureMaps->heightTextureMap != nullptr)	  m_TextureMaps->heightTextureMap->UnBind();
			if (m_TextureMaps->cubeTextureMap != nullptr)  m_TextureMaps->cubeTextureMap->UnBind();
			if (m_TextureMaps->depthTextureMap != nullptr)  m_TextureMaps->depthTextureMap->UnBind();
			if (m_TextureMaps->aoMap != nullptr)  m_TextureMaps->aoMap->UnBind();
			if (m_TextureMaps->roughnessMap != nullptr)	  m_TextureMaps->roughnessMap->UnBind();
			if (m_TextureMaps->mentallicMap != nullptr)	  m_TextureMaps->mentallicMap->UnBind();
		}
	private:
		std::shared_ptr<Shader>		 m_Shader;
		std::shared_ptr<TextureMaps> m_TextureMaps;
		MaterialColor				 m_MaterialColors;
		Props                        m_Props;
	};

}