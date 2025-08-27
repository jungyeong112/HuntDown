#include "pch.h"
#include "CDash_Effect.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CDash_Effect::CDash_Effect()
{
}

CDash_Effect::~CDash_Effect()
{
}

void CDash_Effect::Initialize()
{
	m_tInfo = { 0,0,50.f,50.f };
	Set_BodyFrame(0, 7, 0, 100.f, false);
	m_pTarget = CObjManager::Get_Instance()->Get_Player();
}

int CDash_Effect::Update()
{

	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ActiveFalseTimer(fDeltaTime);

	Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CDash_Effect::LateUpdate()
{
	Move_BodyFrame();
	if (m_tBodyFrame.iStart > m_tBodyFrame.iEnd)
	{
		m_bIsDead = true;
	}
}


void CDash_Effect::Render(HDC hDC)
{
	m_pFrameKey = (m_iPlayerDir == +1) ? L"DashEffect" : L"DashEffect_L";
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	int iOffsetX = (m_iPlayerDir == +1) ? 0 : -0;

	GdiTransparentBlt(hDC,
		m_tRect.left + iOffsetX, m_tRect.top - 50,
		20, 100,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		20.f * m_tBodyFrame.iStart,
		0,
		20.f, 50.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                  //마젠타

}

void CDash_Effect::Release()
{
}
