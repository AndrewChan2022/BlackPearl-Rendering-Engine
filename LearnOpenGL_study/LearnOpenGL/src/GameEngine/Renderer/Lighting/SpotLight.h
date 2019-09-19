#pragma once
#include"Light.h"
class SpotLight:public Light
{
public:
	struct Attenuation { //˥��ϵ��
		float constant;
		float linear;
		float quadratic;
		Attenuation() :constant(1.0f), linear(0.0014f), quadratic(0.000007) {}
		//Ĭ�Ͼ���50 constant(1.0f),linear(0.09f),quadratic(0.032){}
		//Ĭ�Ͼ���3250constant(1.0f),linear(0.0014f),quadratic(0.000007)
		//���https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/
	};
	SpotLight(const glm::vec3& position ,const glm::vec3& direction,float cutoffAngle)
	:m_Position(position),m_Direction(direction),m_CutOffAngle(cutoffAngle)
	{
		Init();
	};
	virtual ~SpotLight()=default;
	virtual void Init() override;

	inline void UpdatePositionAndDirection(const glm::vec3& position, const glm::vec3& direction) 
	{ 
		m_Position = position; m_Direction = direction;
	}
	inline void UpdateCutOffAngle(float angle) { m_CutOffAngle = angle; }
	inline glm::vec3 GetPosition() { return m_Position; }
	inline glm::vec3 GetDirection() { return m_Direction; }
	inline float GetCutOffAngle() { return m_CutOffAngle; }
	inline Attenuation GetAttenuation() const { return m_Attenuation; }

private:
	float m_CutOffAngle;//�й�Ǵ�С
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	Attenuation m_Attenuation;

};

