#pragma once
#include "pch.h"
#include "Texture.h"
#include "BlackPearl/Core.h"
#include <glad/glad.h>

#include "BlackPearl/Config.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace BlackPearl {

	Texture::Texture(Type type, const std::string &image)//TODO::���캯����ͷ�������麯����
	{
		m_Path = image;
		m_Type = type;
		glGenTextures(1, &m_TextureID);
		Bind();
		Init(image);
		

	}

	Texture::Texture(Type type, const int width, const int height,bool isDepth)
	{
		m_Path = "";
		
		m_Type = type;
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		if(isDepth==false)
		Init(width, height);
		


	}

	Texture::Texture(Type type, std::vector<std::string> faces)
	{
		m_Path = "";
		m_FacesPath = faces;
		m_Type = type;
		glGenTextures(1, &m_TextureID);
		//glBindTexture(GL_TEXTURE_2D, m_TextureID);
		//Init(faces);
	}


	void Texture::Init(const std::string &image)
	{
		LoadTexture(image);

	}

	void Texture::Init(const int width, const int height)
	{
		//�������---�ڽ����˺����Թ���
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//������Сʱ���ڽ�����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//����Ŵ�ʱҲ���ڽ�����
	}

	

	void Texture::LoadTexture(const std::string & image)
	{
		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load(true);


		unsigned char *data = stbi_load(image.c_str(), &width, &height, &nrChannels, 0);
		GE_ASSERT(data, "fail to load texture data!");
		GLenum format;
		switch (nrChannels) //ע�ⲻͬͼƬ�в�ͬ��ͨ������
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			GE_CORE_ERROR("Channel {0} has unknown format!", nrChannels)
				break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture::UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}