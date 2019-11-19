#type vertex
#version 330 core
layout (location =0) in vec3 aPos;

uniform mat4 lightProjectionViewMatrix;
uniform mat4 u_Model;

void main(){
	//��ģ���еĶ���任����ռ�
	gl_Position = lightProjectionViewMatrix*u_Model*vec4(aPos,1.0f);
}
#type fragment
#version 330 core
void main(){
	//�����������ɫ��ʲôҲ���ɣ����������Ȼ���ᱻ����
	gl_FragDepth = gl_FragCoord.z;
}