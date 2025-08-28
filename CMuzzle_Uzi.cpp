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
	int iOffsetX = (m_iPlayerDir == +1) ? 5 : 35;
	m_tInfo.fX = m_pTarget->Get_FirePos().fx + iOffsetX;
	m_tInfo.fY = m_pTarget->Get_FirePos().fy + 10;
	Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

