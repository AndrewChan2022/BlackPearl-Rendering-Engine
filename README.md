

1.������պ���

ע��㣺

Camera ��View Matrix��ƽ���������ٶ���պ�����Ч����պ���ֻ����ת����

skybox.glsl:

```
void main()
{
	TexCoords = aPos;
	vec4 pos = u_Projection*mat4(mat3(u_View))* vec4(aPos,1.0);
	gl_Position = pos.xyww;
}

```

����˳��ҲҪע�⣺

�Ȼ��Ʊ������-->����glDepthFunc(GL_LEQUAL)-->������պ���

SkyBoxTestLayer.h

```
	BlackPearl::RenderCommand::SetClearColor(m_BackgroundColor);
	BlackPearl::Renderer::BeginScene(*(m_CameraObj->GetComponent<BlackPearl::PerspectiveCamera>()), *GetLightSources());
		
	std::vector<BlackPearl::Object*> objs;
	objs.push_back(m_SkyBoxObj);

	DrawObjectsExcept(objs);
	//С�ڵ��ڵ�ǰ��Ȼ����fragment�ű�����
	glDepthFunc(GL_LEQUAL);
	DrawObject(m_SkyBoxObj);

	glDepthFunc(GL_LESS);
```