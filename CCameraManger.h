#pragma once
#include "Define.h"
#include "CalculateUtil.h"


class CObj;

class CCameraManger
{
private:
	CCameraManger();
	~CCameraManger();
public:
	CCameraManger(const CCameraManger&) = delete;
	CCameraManger& operator=(const CCameraManger&) = delete;

private:
	Vector2  m_vLookAt;    //카메라가 보는 위치
	CObj* m_pTarget;       // 대상
	Vector2  m_vDiffrence; //해상도 중심 위치와 카메라 LookAt 차이 값 

public:
	void Set_LookAt(Vector2 _vLook) { m_vLookAt = _vLook; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }

public:
	void Update();
	VECTOR2 Get_RenderPos(Vector2 _objPos);

private:
	void Offset();

public:
	static CCameraManger* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CCameraManger;

		return m_pInstance;
	}
	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private:
	static CCameraManger* m_pInstance;
};