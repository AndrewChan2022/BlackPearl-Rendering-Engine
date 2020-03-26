struct Material{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 emissionColor;
	float roughnessValue;
	float mentallicValue;
	float aoValue;
	sampler2D diffuse; //or call it albedo
	sampler2D specular;
	sampler2D emission;
	sampler2D normal;
	sampler2D height;
	sampler2D depth;
	sampler2D ao;
	sampler2D roughness;
	sampler2D mentallic;

	samplerCube cube;
	float shininess;
	float specularDiffusion;
	float diffuseReflectivity;
	float specularReflectivity;
	float transparency;
	float emissivity;
	float refractiveIndex;
};

struct Settings{
	bool isBlinnLight;
	int  isTextureSample;//�ж��Ƿ�ʹ��texture,����ֻ��color
	int isDiffuseTextureSample;
	int isSpecularTextureSample;
	int isMetallicTextureSample;
	bool directLight;
	bool indirectDiffuseLight;
	bool indirectSpecularLight;
	bool shadows;
	float GICoeffs;
	bool hdr;
	bool guassian_horiziotal;
	bool guassian_vertical;
	bool showBlurArea;
};
