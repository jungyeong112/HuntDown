#include "pch.h"
#include "CExplosion.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CExplosion::CExplosion()
{
}

CExplosion::~CExplosion()
{
}

void CExplosion::Initialize()
{
	m_tInfo = { 100.f,100.f,150.f,200.f };
	m_fSpeed = 100.f;
	Set_BodyFrame(0, 9, 0, 100.f,false);
}

int CExplosion::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

	Check_Die(fDeltaTime);


	CObj::Update_Rect();
	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CExplosion::LateUpdate()
{
	Move_BodyFrame();
}

void CExplosion::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Explosion");

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		150, 200,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		80.f * m_tBodyFrame.iStart,
		80.f * m_tBodyFrame.iMotion,
		80.f, 80.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                  //마젠타

	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}
}

void CExplosion::Release()
{
}

void CExplosion::Check_Die(float fDeltaTime)
{
	m_fElapsedTime += fDeltaTime;
	if (m_fElapsedTime >= m_fDeathTime)
	{
		m_bIsDead = true;
	}
}
