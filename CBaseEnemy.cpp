#include "pch.h"
#include "CBaseEnemy.h"
#include "CObjManager.h"

CBaseEnemy::CBaseEnemy()
{
}

CBaseEnemy::~CBaseEnemy()
{
}

void CBaseEnemy::Check_Distance(CObj* pTarget)
{
	if (m_eCurEnemyState != DIE)
	{
		float ftargetX = pTarget->Get_Info().fX;
		if (m_fPlayerRange >= abs(ftargetX - m_tInfo.fX))
		{
			if (m_tInfo.fX > ftargetX)
			{
				OBJID eID = pTarget->Get_ID();
				wstring str = to_wstring(eID) + L"¿Þ ÂÊ OBJ ID : \n";
				m_iPlayerDir = -1;
				OutputDebugString(str.c_str());
			}
			else
			{
				OBJID eID = pTarget->Get_ID();
				wstring str = to_wstring(eID) + L"¿À¸¥ ÂÊ OBJ ID : \n";
				OutputDebugString(str.c_str());
				m_iPlayerDir = +1;
			}
			m_bIsChase = true;
			if (CObjManager::Get_Instance()->Get_Obj_InRange(BOX, m_tInfo.fX, m_tInfo.fY, m_fFind_CoveRange))
				m_bIsCoverCrouch = true;
			else
				m_bIsCoverCrouch = false;
		
		}
		else
		m_bIsChase = false;
	}
}


