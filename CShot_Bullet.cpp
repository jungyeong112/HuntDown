#include "pch.h"
#include "CShot_Bullet.h"
#include "TimeManager.h"
#include "CBmpMgr.h"


CShot_Bullet::CShot_Bullet()
{
}

CShot_Bullet::~CShot_Bullet()
{

}

void CShot_Bullet::Initialize()
{
	m_tInfo = { 100.f, 100.f, 15.f, 6.f };
	m_fSpeed = 800.f;
	Set_BodyFrame(0,1,0,200.f);
}

int CShot_Bullet::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	m_tInfo.fX += (m_iPlayerDir * m_fSpeed * cosf(m_fAngle * (PI / 180.f))) * fDeltaTime;
	m_tInfo.fY -= (m_iPlayerDir * m_fSpeed * sinf(m_fAngle * (PI / 180.f))) * fDeltaTime;
	CObj::Update_Rect();
	Check_Die(fDeltaTime);
	if (!m_bIsDead)
		return OBJ_NO_EVENT;
	else
		return OBJ_DIE;
}

void CShot_Bullet::LateUpdate()
{
	CObj::Move_BodyFrame();
}

void CShot_Bullet::Render(HDC hDC)
{
	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"AK_Bullet");

	//GdiTransparentBlt(hDC,
	//	m_tRect.left, m_tRect.top,
	//	20, 7,                           //12는 피격 박스와 스프라이트 크기 보정
	//	hMemDC,
	//	15.f * m_tBodyFrame.iStart,
	//	8.f * m_tBodyFrame.iMotion,
	//	15.f, 6.f,                    //복사할 가로 세로 사이즈
	//	RGB(255, 0, 255));                  //마젠타

	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

	}
}

void CShot_Bullet::Release()
{
}
