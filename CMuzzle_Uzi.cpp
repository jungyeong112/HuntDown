#include "pch.h"
#include "CMuzzle_Uzi.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CMuzzle_Uzi::CMuzzle_Uzi()
{
}

CMuzzle_Uzi::~CMuzzle_Uzi()
{
}

int CMuzzle_Uzi::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ActiveFalseTimer(fDeltaTime);
	int iOffsetX = 0;
	int iOffsetY = 0;
	if (m_pTarget->Get_ID() == ENEMY) 
	{
		iOffsetX = (m_iPlayerDir == +1) ? -10 : 40;
		iOffsetY = 5;
	}
	else
	{
		iOffsetX = (m_iPlayerDir == +1) ? 5 : 35;
		iOffsetY = 0;
	}
	m_tInfo.fX = m_pTarget->Get_FirePos().fx + iOffsetX;
	m_tInfo.fY = m_pTarget->Get_FirePos().fy + 10 +iOffsetY;
	Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

