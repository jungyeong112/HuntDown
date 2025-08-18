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
				m_iPlayerDir = -1;
			}
			else
			{
				OBJID eID = pTarget->Get_ID();
				m_iPlayerDir = +1;
			}
			m_bIsInRange = true;
			if (CObjManager::Get_Instance()->Get_Obj_InRange(BOX, m_tInfo.fX, m_tInfo.fY, m_fFind_CoveRange))
				m_bIsCoverCrouch = true;
			else
				m_bIsCoverCrouch = false;

		}
		else
			m_bIsInRange = false;
	}
}

void CBaseEnemy::Player_Chase(float fDeltaTime)
{
	float		fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

	fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
	fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;

	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	fRadian = acosf(fWidth / fDiagonal);

	/*if (m_pTarget->Get_Info().fY > m_tInfo.fY)
		fRadian = 2 * PI - fRadian;*/

	m_fAngle = fRadian * (180.f / PI);

	/*if (m_pTarget->Get_Info().fY > m_tInfo.fY)
		m_fAngle *= -1.f;*/

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f))*fDeltaTime;
	//m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f))*fDeltaTime;
}


