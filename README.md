### LightProbes -- Use Spherical Harmonics Coefficients to render diffuse probes

reference paper : "An Efficient Representation for Irradiance Environment Maps"http://graphics.stanford.edu/papers/envmap/envmap.pdf"


reference code :"cubemap-sh" https://github.com/nicknikolov/cubemap-sh

#### 1.���룺��ȡÿ��Probe��EnvironmentCubeMap��6�����pixels
	�����9��Spherical Harmonicsϵ��

```
SphericalHarmonics::UpdateCoeffs(std::shared_ptr<CubeMapTexture> environmentCubeMap)

```

#### 2.ʹ��SH Coefficients draw diffuseIrradiance

lightProbes.glsl:
```

#type fragment
#version 430 core
out vec4 FragColor;
in vec3 TexCoords;
in vec3 v_Normal;


uniform vec3 u_SHCoeffs[9];

vec3 SHDiffuse(const vec3 normal){
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	vec3 result = (
		u_SHCoeffs[0] +
		
		u_SHCoeffs[1] * x +
		u_SHCoeffs[2] * y +
		u_SHCoeffs[3] * z +
		
		u_SHCoeffs[4] * z * x +
		u_SHCoeffs[5] * y * z +
		u_SHCoeffs[6] * y * x +
		u_SHCoeffs[7] * (3.0 * z * z - 1.0) +
		u_SHCoeffs[8] * (x*x - y*y)
  );

  return max(result, vec3(0.0));

}
void main(){
	
    	vec3 N   = normalize(v_Normal);
	vec3 color =SHDiffuse(N);
	color = pow(color,vec3(1.0/2.2));

	FragColor = vec4(color,1.0);
}
	
```
#### bug: 1.probe��SH coefficeints ����ʱ��̫��--�����������thread
	todo: ���probe��diffuseIrradiance�ں�

![SHdiffuselightProbes1](/results/SHdiffuselightProbes1.png)
![SHdiffuselightProbes2](/results/SHdiffuselightProbes2.png)



