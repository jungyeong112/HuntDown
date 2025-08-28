#include "pch.h"
#include "CUIManager.h"

CUIManager* CUIManager::m_pInstance = nullptr;

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
	Release();
}

void CUIManager::Add_UI(UI_ID uId, CUI* pUI)
{
	if (uId >= UI_END || pUI == nullptr)
		return;

	m_UIList[uId].push_back(pUI);
}

void CUIManager::Delete_UI(UI_ID uId)
{
	if (m_UIList[uId].empty())
		return;
	for (auto& pUI : m_UIList[uId]) 
	{
		Safe_Delete(pUI);
	}
	m_UIList[uId].clear();
}

void CUIManager::Update()
{
	for (size_t i = 0; i < UI_END; ++i) 
	{
		for (auto iter = m_UIList[i].begin(); iter != m_UIList[i].end();) 
		{
			int iRes = (*iter)->Update();
			if (iRes == OBJ_DIE)
			{
				Safe_Delete(*iter);
				iter = m_UIList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CUIManager::LateUdate()
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto iter = m_UIList[i].begin(); iter != m_UIList[i].end(); ++iter)
		{
			(*iter)->LateUpdate();
		}
	}
}

void CUIManager::Render(HDC hDC)
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto iter = m_UIList[i].begin(); iter != m_UIList[i].end(); ++iter)
		{
			(*iter)->Render(hDC);
		}
	}
}

void CUIManager::Release()
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for_each(m_UIList[i].begin(), m_UIList[i].end(), Safe_Delete<CUI*>);
		m_UIList[i].clear();
	}
}

void CUIManager::ElapsedClearTime(float fDeltaTime)
{
	if (!m_bIsClear)
		m_fClearTime += fDeltaTime;
}
