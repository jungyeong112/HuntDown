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
		if (abs(ftargetX - m_tInfo.fX) < m_fMeleeRange && m_bIsYHeight)
		{
			m_bIsMelee = true;
		}
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
			if (CObjManager::Get_Instance()->Get_Obj_InRange(BOX, m_tInfo.fX, m_tInfo.fY, m_fFind_CoveRange, m_iPlayerDir))
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

	m_fAngle = fRadian * (180.f / PI);

	bool isJump = CObjManager::Get_Instance()->Get_Obj_InRange(FLAT_GROUND, m_tInfo.fX, m_tInfo.fY, 100.f, m_iPlayerDir);

	if (m_fMeleeRange < fDiagonal && !m_bISKickHit)
	{
		m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f)) * fDeltaTime;
		if ((!m_bIsYHeight && m_iChaseY == -1 && isJump))
		{
			OutputDebugString(L"ChaseJump");
			m_eCurEnemyState = JUMP;
		}
		if (!m_bIsYHeight && m_iChaseY == +1 && m_bIsDownJumpable)
		{
			OutputDebugString(L"아래점프");
			m_tInfo.fY += 40.f;
			m_eCurEnemyState = JUMP;
			m_bIsDownJumpable = false;
		}
	}

}

void CBaseEnemy::Check_TargetY(CObj* pTarget)
{
	if (m_eCurEnemyState != DIE && m_bIsInRange)
	{
		float ftargetY = pTarget->Get_Info().fY;
		if (abs(ftargetY - m_tInfo.fY) < m_tInfo.fCY)
		{
			m_bIsYHeight = true;
			m_iChaseY = 0;
		}
		else
		{
			m_bIsYHeight = false;
			if (ftargetY < m_tInfo.fY)
				m_iChaseY = -1;
			else
				m_iChaseY = +1;
		}

	}
}
