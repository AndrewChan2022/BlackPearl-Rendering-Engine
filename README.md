### ԭ�д����ع� 

* �����ݺͲ�������

ԭ����Draw��Mesh class ���еģ�������ȡ������Meshֻ��ȡ���ݣ�ObjectManagerֻ���𴴽�ɾ��Object

* ����MasterRenderer Class �������е���Ⱦ

MasterRenderer ���ְ���BasicRenderer �� ShadowMapRenderer,

BasicRenderer ��������Ⱦ

ShadowMapRenderer ����FrameBuffer����������Ӱ��ͼ��ShadowMap)

* ��Ҫ��Ӱ��Ⱦ��������:

ShadowMapTextLayer.h
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
![Alt text](/results/image9.png)