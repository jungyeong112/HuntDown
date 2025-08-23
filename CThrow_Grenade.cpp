#include "pch.h"
#include "CThrow_Grenade.h"
#include "CBmpMgr.h"
#include "TimeManager.h"
#include "CAbstarctFactory.h"
#include "CObjManager.h"
#include "CExplosion.h"

CThrow_Grenade::CThrow_Grenade()
{
}

CThrow_Grenade::~CThrow_Grenade()
{
}


void CThrow_Grenade::Initialize()
{
	m_tInfo = { 100.f, 100.f, 30.f, 30.f };

	const float throwSpeed = 520.f;
	m_fAngle = 60.f;

	const float rad = m_fAngle * (PI / 180.f);
	m_vx = m_iPlayerDir * throwSpeed * cosf(rad);
	m_vy = -throwSpeed * sinf(rad);

	m_fElapsedTime = 0.f;
	m_bIsDead = false;

	Set_BodyFrame(0, 6, 0, 200.f);
	CObj::Update_Rect();
}


int CThrow_Grenade::Update()
{
	float dt = TimeManager::GetInstance()->GetDeltaTime();
	if (dt > 0.033f) dt = 0.033f; // dt 제한 

	m_vy += m_gravity * dt;

	m_tInfo.fX += (m_vx * m_iPlayerDir) * dt;
	m_tInfo.fY += m_vy * dt;

	CObj::Update_Rect();

	Check_Die(dt);
	if (m_bIsDead)
	{
		CObjManager::Get_Instance()->Add_Object(
			EXPLOSION, CAbstractFactory<CExplosion>::Create(m_tInfo.fX, m_tInfo.fY - 100.f, m_iPlayerDir));
		return OBJ_DIE;
	}
	return OBJ_NO_EVENT;
}

void CThrow_Grenade::LateUpdate()
{
	Move_BodyFrame();
}

void CThrow_Grenade::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Grenade");

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		15, 20,
		hMemDC,
		6 * m_tBodyFrame.iStart,
		32.f * m_tBodyFrame.iMotion,
		6.f, 8.f,                   //복사할 가로 세로 사이즈
		RGB(255, 0, 255));
	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}
}

void CThrow_Grenade::Release()
{
}

void CThrow_Grenade::OnCollision(FCollision _pCollision)
{
	if (_pCollision.m_OBJID == GROUND || _pCollision.m_OBJID == FLAT_GROUND)
	{
		if (_pCollision.m_Collisiontype & CF_Bottom)
		{
			m_tInfo.fY -= _pCollision.m_fY;  
			CObj::Update_Rect();

			if (m_vy > 0.f) m_vy = -m_vy * m_restitution; // 위로 튕김
			m_vx *= m_friction;                            // 수평 감쇠
		}
	}
}

void CThrow_Grenade::Check_Die(float fDeltaTime)
{
	m_fElapsedTime += fDeltaTime;
	if (m_fElapsedTime >= m_fDeathTime)
	{
		m_bIsDead = true;
	}
}
