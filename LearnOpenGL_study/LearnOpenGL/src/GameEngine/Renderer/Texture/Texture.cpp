#include"pch.h"
#include "Texture.h"
#include"GameEngine/Core.h"
#include <glad/glad.h>
#include "stb_image.h"



Texture::Texture(const char *image)
{
	
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	//set the texture warpping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//����s��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//����t��

	//�������---�ڽ����˺����Թ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//������Сʱ���ڽ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//����Ŵ�ʱҲ���ڽ�����

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);//������С��ʱ��ʹ�����ڽ�Mipmapƥ�����ش�С����ʹ���ڽ���ֵ�����������
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//����Ŵ�ʱҲ���ڽ�����

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(image, &width, &height, &nrChannels, 0);
	GE_ASSERT(data, "fail to load texture data!")

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
		GE_CORE_ERROR("Channel {0} has unknown format!")
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
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
