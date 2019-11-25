### ���PointLight shadow map

* ��PointLightΪ�ӽǣ���CubeMap��ȡ�����ͼ,ͨ��������ɫ����һ��frangmentӳ�䵽CubeMap��������,�����Դ������fragment����ȣ�����CubeMap

Shader Code:
CubeMapDepthShader.glsl

```
#type vertex
#version 330 core
layout (location =0) in vec3 aPos;

uniform mat4 u_Model;
void main(){
	gl_Position = u_Model * vec4(aPos,1.0);
}

#type geometry
#version 330 core
layout (triangles) in;
layout (triangle_strip,max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;

void main(){

	for(int face = 0;face<6;++face){
		gl_Layer = face;
		for(int i=0;i<3;++i){
			FragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face]*FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}

//����ÿ��Fragment����Դ�����
#type fragment
#version 330 core
in vec4 FragPos;

uniform vec3 u_LightPos;
uniform float u_FarPlane;

void main(){
    // get distance between fragment and light source
	float lightDistance = length(FragPos.xyz-u_LightPos);
	
	//map to [0:1] range by dividing by farPlane;
	lightDistance = lightDistance/u_FarPlane;

	gl_FragDepth = lightDistance;
	
}

```

### ����Shadow

* �� Direct Light ShadowMap һ��������PointLight��fragment�ľ��룬����CubeMap�Աȡ�
CubeMapShadowMapping.glsl
```


		InputCheck(ts);

		//// render

		BlackPearl::RenderCommand::SetClearColor(m_BackgroundColor);
		BlackPearl::Renderer::BeginScene(*(m_CameraObj->GetComponent<BlackPearl::PerspectiveCamera>()), *GetLightSources());


		for (BlackPearl::Object* obj : m_ObjectsList) {
			if (obj!=Quad&&obj->HasComponent<BlackPearl::MeshRenderer>())
				obj->GetComponent<BlackPearl::MeshRenderer>()->SetShaders(m_SimpleDepthShader);
		}


		m_MasterRenderer.RenderShadowMap(m_ObjectsList, m_Sun->GetComponent<BlackPearl::ParallelLight>(), {Quad});
		// 2. Render scene as normal 
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Shader->Bind();
		m_Shader->SetUniformVec3f("u_LightPos", m_Sun->GetComponent<BlackPearl::ParallelLight>()->GetDirection());
		m_Shader->SetUniformMat4f("u_LightProjectionViewMatrix", m_MasterRenderer.GetShadowMapLightProjectionMatrx());
		for (BlackPearl::Object* obj : m_ObjectsList) {
			if (obj != Quad && obj->HasComponent<BlackPearl::MeshRenderer>())
			obj->GetComponent<BlackPearl::MeshRenderer>()->SetShaders(m_Shader);
		}
		
		m_MasterRenderer.RenderSceneExcept(m_ObjectsList, Quad, GetLightSources() );

		glViewport(0, 0, 480, 270);
		Quad->GetComponent<BlackPearl::MeshRenderer>()->GetMeshes()[0].GetMaterial()->GetShader()->Bind();
		Quad->GetComponent<BlackPearl::MeshRenderer>()->GetMeshes()[0].GetMaterial()->GetShader()->SetUniformVec3f("u_LightPos", m_Sun->GetComponent<BlackPearl::ParallelLight>()->GetDirection());
		Quad->GetComponent<BlackPearl::MeshRenderer>()->GetMeshes()[0].GetMaterial()->GetShader()->SetUniformMat4f("u_LightProjectionViewMatrix", m_MasterRenderer.GetShadowMapLightProjectionMatrx());


		m_MasterRenderer.RenderObject(Quad);


	}


```
![Alt text](/results/Image new1.png)