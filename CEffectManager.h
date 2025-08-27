#pragma once
#include "CEffect.h" 

class CEffectManager
{

public:
	CEffectManager();
	~CEffectManager();

public:
	void Add_EFFECT(EFFECTID eId, CEffect* pEffect);
	void Delete_EFFECT(EFFECTID eId);
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	static CEffectManager* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CEffectManager();
		}
		return m_pInstance;
	}
	static void Destory_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

protected:
	list<CEffect*> m_EffectList[EFFECT_END];
	static CEffectManager* m_pInstance;
};

