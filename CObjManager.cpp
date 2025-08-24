#include "pch.h"
#include "CObjManager.h"
#include "CPlayer.h"

CObjManager* CObjManager::m_pInstance = nullptr;

CObjManager::CObjManager()
{
}

CObjManager::~CObjManager()
{
	Release();
}

void CObjManager::Add_Object(OBJID eId, CObj* pObj)
{
	if (eId >= OBJ_END || pObj == nullptr)
		return;

	m_ObjList[eId].push_back(pObj);
}

void CObjManager::Delete_Object(OBJID eId)
{
	if (m_ObjList[eId].empty())
		return;
	for (auto& pobj : m_ObjList[eId])
	{
		Safe_Delete(pobj);
	}
	m_ObjList[eId].clear();
}

void CObjManager::Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();)
		{
			int iRes = (*iter)->Update();
			if (iRes == OBJ_DIE)
			{
				Safe_Delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void CObjManager::LateUpdate()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); ++iter)
		{
			(*iter)->LateUpdate();
		}
	}
}

void CObjManager::Render(HDC hDC, POINT pt)
{	
	RECT camRect = CScreenManager::Instance().GetCamRect();
	RECT inter;
		
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); ++iter)
		{
			RECT objRect = (*iter)->Get_Rect();
			if (IntersectRect(&inter,& objRect,&camRect))
				(*iter)->Render(hDC);
		}
	}
}

void CObjManager::Release()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

Vector2 CObjManager::Get_PlayerPos()
{

	Vector2 vPos = { m_ObjList[PLAYER].front()->Get_Info().fX, m_ObjList[PLAYER].front()->Get_Info().fY };

	return vPos;
}

CObj* CObjManager::Get_Player()
{
	return m_ObjList[PLAYER].front();
}

CObj* CObjManager::Get_Obj_InRange(OBJID eId, float fx, float fy, float fRange, int iDir)
{
	for (auto& pObj : m_ObjList[eId])
	{
		
		float dX = pObj->Get_Info().fX - fx;
		float dY = pObj->Get_Info().fY - fy;
		if (iDir == +1)
		{
			if (dX > 0)
			{
				if (dX * dX + dY * dY <= fRange * fRange)
				{
					return pObj;
				}
			}
			else
				continue;
		}
		else if(iDir == -1)
		{
			if (dX < 0)
			{
				if (dX * dX + dY * dY <= fRange * fRange)
				{
					return pObj;
				}
			}
			else
				 return nullptr;
		}
	}
}

