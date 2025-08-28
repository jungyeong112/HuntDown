#include "pch.h"
#include "CLandEffect.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CLandEffect::CLandEffect()
{
}

CLandEffect::~CLandEffect()
{
}

void CLandEffect::Initialize()
{
	m_tInfo = { 0,0,50.f,50.f };
	Set_BodyFrame(0, 3, 0, 150.f, false);
	m_pTarget = CObjManager::Get_Instance()->Get_Player();
	Update_Rect();
}

int CLandEffect::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ActiveFalseTimer(fDeltaTime);

	Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CLandEffect::LateUpdate()
{
	Move_BodyFrame();
	if (m_tBodyFrame.iStart > m_tBodyFrame.iEnd)
	{
		m_bIsDead = true;
	}
}

void CLandEffect::Render(HDC hDC)
{
	
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"LandEffect");

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top + 30,
		60, 40,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		20.f * m_tBodyFrame.iStart,
		0,
		50.f, 17.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));
}

void CLandEffect::Release()
{
}
