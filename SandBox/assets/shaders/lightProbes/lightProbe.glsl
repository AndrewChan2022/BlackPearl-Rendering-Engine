#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;
		
uniform mat4 u_Model;
uniform mat4 u_ProjectionView;
uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	TexCoords = aPos;
	gl_Position = u_ProjectionView*u_Model*vec4(aPos,1.0);

	//gl_Position = pos.xyww;
	/*
	
	������ϵͳ�̳��У�����˵���ڶ�����ɫ�����к�ִ��͸�ӻ��֣�
	����gl_Position��xyz�����������w������
	���ǻ�����Ȳ��Խ̳��е�֪�����շָ��z���������Ǹ���������ֵ��
	���������Ϣ�����ǿ����������λ�õ�z������������w������
	�⽫����z�������ǵ���1.0����Ϊ��Ӧ��͸�ӳ���ʱ��
	����z����ת����w / w = 1.0:
	ʹ��sykbox��λ��һֱ�����ͷ
	
	*/
}



#type fragment
#version 330 core
out vec4 FragColor;
in vec3 TexCoords;

struct Material{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 emissionColor;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	sampler2D normal;
	sampler2D height;
	samplerCube cube;
	float shininess;

};
uniform Material u_Material;

float near = 0.1; 
float far  = 100.0; 
float LinearizeDepth(float depth){
	float z = depth*2.0-1.0;//Back to NDC coordinate
	return 2.0*near*far /(far+near - z*(far - near));

}
void main(){
	//FragColor = vec4(u_Material.diffuseColor,1.0);
	//FragColor = vec4(1.0,1.0,0.1,1.0);//texture(u_Material.cube,TexCoords);//*vec4(u_Material.diffuseColor,1.0);
	//vec3 color =texture(u_Material.cube,TexCoords).rgb;
		FragColor =texture(u_Material.cube,TexCoords);//vec4(color,1.0);

//	float depth = LinearizeDepth(gl_FragCoord.z)/far;
//	FragColor=vec4(vec3(depth),1.0);
}
