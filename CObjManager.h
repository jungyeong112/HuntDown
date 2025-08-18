#pragma once
#include "CObj.h"

class CObjManager
{
public:
	CObjManager();
	~CObjManager();

public:

	void Add_Object(OBJID eId, CObj* pObj);
	void Delete_Object(OBJID eId);
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();
	list<CObj*>* Get_List() { return m_ObjList; }
	Vector2  Get_PlayerPos();

public:
	static CObjManager* Get_Instance()
	{
		if (m_pInstance==nullptr) 
		{
			m_pInstance = new CObjManager;
		}
		return m_pInstance;
	}
	static void Destory_Instance() 
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

protected:
	list<CObj*> m_ObjList[OBJ_END];
	static CObjManager* m_pInstance;
};

