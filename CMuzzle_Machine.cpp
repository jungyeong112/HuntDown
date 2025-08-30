#include "pch.h"
#include "CMuzzle_Machine.h"
#include "TimeManager.h"
#include "CBmpMgr.h"

CMuzzle_Machine::CMuzzle_Machine()
{
}

CMuzzle_Machine::~CMuzzle_Machine()
{
}

void CMuzzle_Machine::Initialize()
{
	m_tInfo = { 0,0,20.f,25.f };
	Set_BodyFrame(0, 2, 0, 50.f, false);
}

int CMuzzle_Machine::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ActiveFalseTimer(fDeltaTime);
	m_tInfo.fX = m_pTarget->Get_FirePos().fx + 20.f + (m_pTarget->Get_EffectOffset().fx * -m_iPlayerDir);
	m_tInfo.fY = m_pTarget->Get_FirePos().fy + m_pTarget->Get_EffectOffset().fy;
	Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CMuzzle_Machine::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MachineMuzzle");

	GdiTransparentBlt(hDC,
		m_tRect.left-120, m_tRect.top+53,
		50, 50,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		35.f * m_tBodyFrame.iStart,
		0,
		35.f, 35.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                  //마젠타
}
