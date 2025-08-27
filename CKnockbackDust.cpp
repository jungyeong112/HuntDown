#include "pch.h"
#include "CKnockbackDust.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CKnockbackDust::CKnockbackDust()
{
}

CKnockbackDust::~CKnockbackDust()
{
}

void CKnockbackDust::Initialize()
{
	m_tInfo = { 0,0,20.f,20.f };
	Set_BodyFrame(0, 4, 0, 60.f, false);
}

int CKnockbackDust::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ActiveFalseTimer(fDeltaTime);
	Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CKnockbackDust::LateUpdate()
{
	Move_BodyFrame();
	if (m_tBodyFrame.iStart > m_tBodyFrame.iEnd)
	{
		m_bIsDead = true;
	}
}

void CKnockbackDust::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Knockback_Dust");

	for (int i = -1; i < 2; i++) 
	{
		int iOffSetX = 15 * i;
		int iOffsetY = (i == 0) ? 5 : 15;
		GdiTransparentBlt(hDC,
			m_tRect.left+ iOffSetX, m_tRect.top+iOffsetY,
			30, 30,                           //12는 피격 박스와 스프라이트 크기 보정
			hMemDC,
			32.f * m_tBodyFrame.iStart,
			0,
			32.f, 32.f,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}
	
}

void CKnockbackDust::Release()
{
}
