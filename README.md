

1.���FrameBuffer

2.��FrameBuffer�е�TextureBuffer��Ϊ��ͼ������������CubeObj��

3.��Ҫ�������£�

	��ʼ����
```
	  	m_PlaneObj= Layer::CreatePlane();
	
		m_CubeObj = Layer::CreateCube();
		

		//��FrameBuffer�е�texture��Ϊ��ͼ������m_CubeObj��
		auto CubemeshComponent = m_CubeObj->GetComponent<BlackPearl::MeshRenderer>();
		CubemeshComponent->SetTexture(0, m_FrameBuffer->GetColorTexture());
```

	Updateѭ����
```
		m_FrameBuffer->Bind(960,540);

		glEnable(GL_DEPTH_TEST);

		BlackPearl::RenderCommand::SetClearColor(m_BackgroundColor);

		BlackPearl::Renderer::BeginScene(*(m_CameraObj->GetComponent<BlackPearl::PerspectiveCamera>()), *GetLightSources());
		

		
		DrawObjectsExcept(m_CubeObj);
		

		m_FrameBuffer->UnBind();
		

		glClearColor(0.0f, 0.1f, 0.1f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

		glViewport(0, 0, 960, 540);
		

		DrawObjects();
```
4.FrameBuffer ѧϰ���Ӳο���https://www.youtube.com/watch?v=21UsMuFTN0k&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh&index=2
		