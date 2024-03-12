#pragma once
#include "BlackPearl/RHI/RHITexture.h"
#include <vulkan/vulkan.h>

namespace BlackPearl {
    //����ETexture���壬��ֹ��OpenGL�����Texture�ظ�
    class ETexture : public RefCounter<ITexture>
    {
    public:

        TextureDesc desc;
        VkImageCreateInfo imageInfo;
        VkExternalMemoryImageCreateInfo externalMemoryImageInfo;
        VkImage image;
        VkImageView view;

        ~ETexture() override {}
        const TextureDesc& getDesc() const override { return desc; }
       

    private:
  
    private:
    };

}

