#pragma once
#include "CUI.h"
#include "CPlayer.h"
class CUIManager
{
public:
	CUIManager();
	~CUIManager();

public:
	void Set_Player(CObj* pObj) { m_pPlayer = pObj; }
	CObj* Get_Player() { return m_pPlayer; }
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
	CObj* m_pPlayer;
};

