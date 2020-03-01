###  Voxel Cone Tracing �Ľ�--�������Χ�������壩���ػ�

#### 1.��������� CubeSize �涨���ػ���Χ��ֻ�����������ڵĲ����ػ�

#### 2.���ڲ����ǶԵ�λ������voxelization�ˣ������Ҫ����仯����

CubeSize * CubeSize *CubeSize�ĳ��������ػ��� 1*1*1��voxel�ڣ�

����׷��ʱ���ٴ� 1*1*1 voxel�Ŵ�ԭΪԭ������


voxelization.glsl

```
void main(){
	vec3 viewDir = normalize(u_CameraViewPos-worldPositionFrag);

	vec3 color = vec3(0.0);
	if(!isInsideCube(worldPositionFrag, 0)) return;
	vec3 normalWorldPositionFrag =worldPositionFrag-u_CameraViewPos;

 	normalWorldPositionFrag = normalWorldPositionFrag/u_CubeSize;
	for(int i = 0; i < u_PointLightNums; ++i) 
	{
	color += CalcPointLight(u_PointLights[i], normalFrag,viewDir);

	}

	// Output lighting to 3D texture.
	vec3 voxel = scaleAndBias(normalWorldPositionFrag);
	ivec3 dim = imageSize(texture3D);// retrieve the dimensions of an image
	float alpha = pow(1 - 0, 4); // For soft shadows to work better with transparent materials.
	vec4 res = alpha * vec4(vec3(color), 1);
    imageStore(texture3D, ivec3(dim * voxel), res);//write a single texel into an image;
}

```
#### 3.���ڵ�bug --> 1������ CubeSize�������ػ��ֱ���Խ��
2��tracing�������⡣�����������



![voxelization](/results/voxelization2.png)

voxelization-CubeSize=1

![voxelization-CubeSize=1](/results/voxelization-CubeSize=1.png)
![voxelization-CubeSize=1_tracing](/results/voxelization-CubeSize=1_tracing.png)

voxelization-CubeSize=2
![voxelization-CubeSize=2](/results/voxelization-CubeSize=2.png)
![voxelization-CubeSize=2_tracing](/results/voxelization-CubeSize=2_tracing.png)

voxelization-CubeSize=3
![voxelization-CubeSize=3](/results/voxelization-CubeSize=3.png)

