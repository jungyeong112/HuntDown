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
	m_tInfo = { 100.f, 100.f ,30.f,30.f };
	m_fSpeed = 00.f;
	m_fAngle = 84.f;
	Set_BodyFrame(0, 6, 0, 200.f);
}

int CThrow_Grenade::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

	float fGravity = 980.f;

	m_tInfo.fY -= ((m_fSpeed * m_fThrowTime) - (0.5f * (fGravity * m_fThrowTime * m_fThrowTime))) * fDeltaTime;
	m_tInfo.fX += (m_iPlayerDir * m_fThorwSpeed * cosf(m_fAngle * (PI / 180.f))) * fDeltaTime;

	m_fThrowTime += fDeltaTime;

	CObj::Update_Rect();
	Check_Die(fDeltaTime);
	if (m_bIsDead)
	{
		CObjManager::Get_Instance()->Add_Object(EXPLOSION, CAbstractFactory<CExplosion>::Create(m_tInfo.fX, m_tInfo.fY - 100.f, m_iPlayerDir));
		return OBJ_DIE;
	}
	else
		return OBJ_NO_EVENT;
}

void CThrow_Grenade::LateUpdate()
{
	Move_BodyFrame();
}

void CThrow_Grenade::Render(HDC hdc)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Grenade");

	GdiTransparentBlt(hdc,
		m_tRect.left, m_tRect.top,
		15, 20,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		6 * m_tBodyFrame.iStart,
		32.f * m_tBodyFrame.iMotion,
		6.f, 8.f,                   //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                  //마젠타

	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hdc, GetStockObject(WHITE_PEN));

		Rectangle(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);
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
			{
				m_fSpeed = 15;
				m_tInfo.fY -= _pCollision.m_fY * 30;
				m_fThrowTime = 0.f;
				m_fThorwSpeed = 200.f;
			}
		}
	}
	CObj::Update_Rect();
}

void CThrow_Grenade::Check_Die(float fDeltaTime)
{
	m_fElapsedTime += fDeltaTime;
	if (m_fElapsedTime >= m_fDeathTime)
	{
		m_bIsDead = true;
	}
}
