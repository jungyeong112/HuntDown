#include "pch.h"
#include "CShot_BulletEffect.h"
#include "TimeManager.h"
#include "CBmpMgr.h"

CShot_BulletEffect::CShot_BulletEffect()
{
}

CShot_BulletEffect::~CShot_BulletEffect()
{
}

void CShot_BulletEffect::Initialize()
{
	m_tInfo = { 0,0, 60.f, 50.f };
	Set_BodyFrame(0, 6, 0, 200.f);
}

int CShot_BulletEffect::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	m_tInfo.fX += (m_iPlayerDir * m_fSpeed) * fDeltaTime;

	CObj::Update_Rect();
	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CShot_BulletEffect::LateUpdate()
{
	Move_BodyFrame();
}

void CShot_BulletEffect::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Shotgun_BulletEffect");

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		60, 50,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		60.f * m_tBodyFrame.iStart,
		50.f * m_tBodyFrame.iMotion,
		60, 50,                    //복사할 가로 세로 사이즈
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

void CShot_BulletEffect::Release()
{
}

void CShot_BulletEffect::OnCollision(FCollision _pCollision)
{
}
