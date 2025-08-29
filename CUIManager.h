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
	void  Set_Clear() { m_bIsClear = true; }
	void  Increase_Collection() { ++m_iCollection; }

	int  Get_Collection() { return m_iCollection; }
	CPlayer* Get_Player() { return m_pPlayer; }
	int Get_EnemyKill() { return m_iEnemyKill; }
	float Get_ClearTime() { return m_fClearTime; }
	bool  Get_Clear() { return  m_bIsClear; }
	int   Get_Die() { return m_iPlayerDie; }
	void  Add_Die() { ++m_iPlayerDie; }
	void  Set_ClearFalse() { m_bIsClear = false; }
	
	void Increas_EnemyKill() { ++m_iEnemyKill; }
	void Add_UI(UI_ID uId, CUI* pUI);
	void Delete_UI(UI_ID uId);
	void Update();
	void LateUdate();
	void Render(HDC hDC);
	void Release();
	void ElapsedClearTime(float fDeltaTime);

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
	int m_iCollection{ 0 };
	int m_iPlayerDie{ 0 };
	float m_fClearTime{ 0.f };
	bool m_bIsClear{ false };
};

