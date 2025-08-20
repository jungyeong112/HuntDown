#include "pch.h"
#include "CEnemyMelee.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CEnemyMelee::CEnemyMelee()
{
}

CEnemyMelee::~CEnemyMelee()
{
}

void CEnemyMelee::Initialize()
{
	m_tInfo = { 100.f, 100.f, 20.f, 20.f };
}

int CEnemyMelee::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	CObj::Update_Rect();
	Check_Die(fDeltaTime);
	if (!m_bIsDead)
		return OBJ_NO_EVENT;
	else
		return OBJ_DIE;
}

void CEnemyMelee::LateUpdate()
{
}

void CEnemyMelee::Render(HDC hDC)
{
	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}
}

void CEnemyMelee::Release()
{
}

void CEnemyMelee::OnCollision(FCollision _pCollision)
{
	if (_pCollision.m_OBJID == PLAYER)
	{
		m_bIsDead = true;
	}
}

void CEnemyMelee::Check_Die(float fDeltaTime)
{
	m_fElapsedTime += fDeltaTime;
	if (m_fElapsedTime >= m_fDeathTime)
	{
		m_bIsDead = true;
	}
}
