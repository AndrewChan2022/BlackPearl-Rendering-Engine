#pragma once
#include "pch.h"
#include "Texture.h"
#include "BlackPearl/Core.h"
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
	//Use in ShadowMap (DepthMap) or FrameBuffer's empty ColorMap
	Texture::Texture(Type type, const int width, const int height,bool isDepth,
		unsigned int minFilter, unsigned int maxFilter,
		int internalFormat, int format,
		int wrap,
		unsigned int dataType
		)
	{
		m_Path = "";
		m_Type = type;
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		if(isDepth==false)
			Init(width, height,minFilter,maxFilter,internalFormat, format, wrap, dataType);
		


	}
	//Use in CubeMap
	Texture::Texture(Type type, std::vector<std::string> faces)
	{
		m_Path = "";
		m_FacesPath = faces;
		m_Type = type;
		glGenTextures(1, &m_TextureID);
	
	}


	void Texture::Init(const std::string &image)
	{
		LoadTexture(image);

	}

	void Texture::Init(const int width, const int height,unsigned int minFilter, unsigned int maxFilter,
		int internalFormat, int format,
		int wrap,
		unsigned int dataType)
	{
	/*	FBO(
			GLuint w, GLuint h, GLenum magFilter = GL_NEAREST, GLenum minFilter = GL_NEAREST,
			GLint internalFormat = GL_RGB16F, GLint format = GL_FLOAT, GLint wrap = GL_REPEAT);*/
		//�������---�ڽ����˺����Թ���
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);//������Сʱ���ڽ�����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);//����Ŵ�ʱҲ���ڽ�����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, NULL);

	}

	

	void Texture::LoadTexture(const std::string & image)
	{
		GE_ASSERT(image.size()!=0, "texture image is empty!");

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
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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