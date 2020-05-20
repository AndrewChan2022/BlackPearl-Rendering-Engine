#pragma once
#include<bitset>
namespace BlackPearl {
	//һ��Entity���32��Component

	//inline static void SyncGPU();
	class Configuration {
	public:
		static const unsigned int MaxComponents = 32;
		typedef std::bitset<MaxComponents> ComponentMask;

		static const unsigned int WindowWidth = 960;
		static const unsigned int WindowHeight = 540;

		//���ز���
		static const unsigned int MSAA_SAMPLES = 4;

	

		//static const VoxelConeTracingRenderer::RenderingMode RenderingMode; 

		// ------------------------------------------
		// Animaition Joints conut
		// ------------------------------------------
		static const int MaxJointsCount = 70;


		// ------------------------------------------
		// Max pointLight Num
		// ------------------------------------------
		static const unsigned int MaxPointLightNum = 20;




		// ------------------------------------------
		// Shadow map
		// ------------------------------------------
		/*����ÿ�� PointLight �� m_ShadowRaduis ���巶Χ�ڵ�����Żử��shadow */
		static const float ShadowMapPointLightRadius;

		// ------------------------------------------
		// light probe environment map	and specular map resolution
		// ------------------------------------------
		/* prefilterMap.glsl���	float resolution =512.0;ҲҪ�� */
		static const float EnvironmantMapResolution;
		//static const float GICoeefs;

		static void SyncGPU();

		// ------------------------------------------
		// Map config
		// ------------------------------------------
		static const unsigned int MapSize = 100;
		static const unsigned int AreaSize = 10;



	};
	struct ShaderConfig {


		static const char* AMBIENT_COLOR;
		static const char* DIFFUSE_COLOR;
		static const char* SPECULAR_COLOR;
		static const char* EMISSION_COLOR;

		static const char* METALLIC_VALUE;
		static const char* ROUGHNESS_VALUE;
		static const char* AO_VALUE;

		static const char* DIFFUSE_TEXTURE2D;
		static const char* SPECULAR_TEXTURE2D;
		static const char* EMISSION_TEXTURE2D;
		static const char* NORMAL_TEXTURE2D;
		static const char* HEIGHT_TEXTURE2D;
		static const char* AO_TEXTURE2D;
		static const char* ROUGHNESS_TEXTURE2D;
		static const char* METALLIC_TEXTURE2D;
		static const char* DEPTH_TEXTURE2D;

		static const char* CUBE_TEXTURECUBE;
		static const char* SHININESS;

		static const char* IS_BLINNLIGHT;
		static const char* IS_PBR_TEXTURE_SAMPLE;
		static const char* IS_AMBIENT_TEXTURE_SAMPLE;
		static const char* IS_DIFFUSE_TEXTURE_SAMPLE;
		static const char* IS_SPECULAR_TEXTURE_SAMPLE;
		static const char* IS_METALLIC_TEXTURE_SAMPLE;
		static const char* IS_EMISSION_TEXTURE_SAMPLE;
		static const char* IS_HEIGHT_TEXTURE_SAMPLE;

	};


}