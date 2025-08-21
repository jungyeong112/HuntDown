#pragma once
#include "CUI.h"
#include "CPlayer.h"
class CUIManager
{
public:
	CUIManager();
	~CUIManager();

public:
	void Set_Player(CPlayer* pObj) { m_pPlayer = pObj; }
	CPlayer* Get_Player() { return m_pPlayer; }
	int Get_EnemyKill() { return m_iEnemyKill; }

	void Increas_EnemyKill() { ++m_iEnemyKill; }
	void Add_UI(UI_ID uId, CUI* pUI);
	void Delete_UI(UI_ID uId);
	void Update();
	void LateUdate();
	void Render(HDC hDC);
	void Release();

public:
	static CUIManager* Get_Instance() 
	{
		if (m_pInstance == nullptr) 
		{
			m_pInstance = new CUIManager();
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
	list<CUI*> m_UIList[UI_END];
	static CUIManager* m_pInstance;
	CPlayer* m_pPlayer;
	int m_iEnemyKill{ 0 };
};

