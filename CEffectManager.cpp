#include "pch.h"
#include "CEffectManager.h"

CEffectManager* CEffectManager::m_pInstance = nullptr;

CEffectManager::CEffectManager()
{
}

CEffectManager::~CEffectManager()
{
	Release();
}

void CEffectManager::Add_EFFECT(EFFECTID eId, CEffect* pEffect)
{
	if (eId >= EFFECT_END || pEffect == nullptr)
		return;

	m_EffectList[eId].push_back(pEffect);
}

void CEffectManager::Delete_EFFECT(EFFECTID eId)
{
	if (m_EffectList[eId].empty())
		return;
	for (auto& pEffect : m_EffectList[eId]) 
	{
		Safe_Delete(pEffect);
	}
	m_EffectList[eId].clear();
}

void CEffectManager::Update()
{
	for (size_t i = 0; i < EFFECT_END; ++i)
	{
		for (auto iter = m_EffectList[i].begin(); iter != m_EffectList[i].end();)
		{
			int iRes = (*iter)->Update();
			if (iRes == OBJ_DIE)
			{
				Safe_Delete(*iter);
				iter = m_EffectList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CEffectManager::LateUpdate()
{
	for (size_t i = 0; i < EFFECT_END; ++i)
	{
		for (auto iter = m_EffectList[i].begin(); iter != m_EffectList[i].end(); ++iter)
		{
			(*iter)->LateUpdate();
		}
	}
}

void CEffectManager::Render(HDC hDC)
{
	for (size_t i = 0; i < EFFECT_END; ++i)
	{
		for (auto iter = m_EffectList[i].begin(); iter != m_EffectList[i].end(); ++iter)
		{
			(*iter)->Render(hDC);
		}
	}
}

void CEffectManager::Release()
{
	for (size_t i = 0; i < EFFECT_END; ++i)
	{
		for_each(m_EffectList[i].begin(), m_EffectList[i].end(), Safe_Delete<CEffect*>);
		m_EffectList[i].clear();
	}
}
