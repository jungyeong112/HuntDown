#include "pch.h"
#include "CMelee.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CMelee::CMelee()
{
}

CMelee::~CMelee()
{
}

void CMelee::Initialize()
{
	m_tInfo = { 100.f,100.f, 40.f,40.f };
}

int CMelee::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	CObj::Update_Rect();
	Check_Die(fDeltaTime);
	if (!m_bIsDead)
		return OBJ_NO_EVENT;
	else
		return OBJ_DIE;
}

void CMelee::LateUpdate()
{
}

void CMelee::Render(HDC hDC)
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

void CMelee::Release()
{
}

void CMelee::OnCollision(FCollision _pCollision)
{
	if (_pCollision.m_OBJID == ENEMY)
	{
		m_bIsDead = true;
	}
}


void CMelee::Check_Die(float fDeltaTime)
{
	m_fElapsedTime += fDeltaTime;
	if (m_fElapsedTime >= m_fDeathTime)
	{
		m_bIsDead = true;
	}
}
