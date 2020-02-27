###  LightProbe + GBuffer + AnimatedModel+shadowMap deferred rendering


#### 1.Ϊÿ��lightProbe���ɻ�����ͼ ��һ�μ��ɣ�

#### 2.Ϊÿ��PointLight����ShadowMap

#### 3.��� lightProbes��ȡ������-GI ,��Ⱦ objects���Ե� gBuffer

#### 4.��gBuffer��ȡ���� -����Ⱦ GI pass,pointLight pass ��frameBuffer

#### 5.frameBuffer �����ڴ��� --������HDR ,tone Mapping

#### 6.���Animated Model ��dynamic objects�� ʹ��AnimatedModelRenderer ��Ⱦ

GBufferRenderer.cpp -->RenderSceneWithGBufferAndProbes()

IBLProbesRenderingLayer.h :

```
	void OnUpdate(BlackPearl::Timestep ts) override {


		InputCheck(ts);

		/*render scene*/
		BlackPearl::RenderCommand::SetClearColor(m_BackgroundColor);
		BlackPearl::Renderer::BeginScene(*(m_MainCamera->GetObj()->GetComponent<BlackPearl::PerspectiveCamera>()), *GetLightSources());


		if (BlackPearl::Input::IsKeyPressed(BP_KEY_U)) {

			m_IBLProbesRenderer->Render(GetLightSources(), m_BackGroundObjsList, m_LightProbes, m_SkyBoxObj1);

		}

		milliseconds currentTimeMs = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		double runtime = currentTimeMs.count()-m_StartTimeMs.count();
		


		
		
		glViewport(0, 0, BlackPearl::Configuration::WindowWidth, BlackPearl::Configuration::WindowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		m_ShadowMapPointLightRenderer->RenderCubeMap(m_SphereObjsList,m_DynamicObjsList,runtime / 1000.0f,GetLightSources());
		m_GBufferRenderer->RenderSceneWithGBufferAndProbes(m_SphereObjsList, m_DynamicObjsList, runtime / 1000.0f,m_BackGroundObjsList, m_GBufferDebugQuad, GetLightSources(), m_LightProbes, m_IBLProbesRenderer->GetSpecularBrdfLUTTexture(), m_SkyBoxObj1);
		m_IBLProbesRenderer->RenderProbes(m_LightProbes);


	}
```

![LightProbes+gBuffer+Shadow+Animation1](/results/LightProbes+gBuffer+Shadow+Animation1.png)
![LightProbes+gBuffer+Shadow+Animation](/results/LightProbes+gBuffer+Shadow+Animation2.png)
