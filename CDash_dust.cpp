#include "pch.h"
#include "CDash_dust.h"
#include "TimeManager.h"
#include "CBmpMgr.h"


CDash_dust::CDash_dust()
{
}

CDash_dust::~CDash_dust()
{
	
}

void CDash_dust::Initialize()
{
	m_tInfo = { 0,0,20.f,20.f };
	Set_BodyFrame(0, 7, 0, 100.f, false);
	m_pTarget = CObjManager::Get_Instance()->Get_Player();
	Update_Rect();
}

int CDash_dust::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ActiveFalseTimer(fDeltaTime);
	Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CDash_dust::LateUpdate()
{
	Move_BodyFrame();
	if (m_tBodyFrame.iStart > m_tBodyFrame.iEnd)
	{
		m_bIsDead = true;
	}
}

void CDash_dust::Render(HDC hDC)
{
	m_pFrameKey = (m_iPlayerDir == +1) ? L"Dashdust" : L"Dashdust_L";
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	int iOffsetX = (m_iPlayerDir == +1) ? 0 : -0;

	GdiTransparentBlt(hDC,
		m_tRect.left + iOffsetX, m_tRect.top + 30,
		30, 30,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		20.f * m_tBodyFrame.iStart,
		0,
		10.f, 15.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));
}

void CDash_dust::Release()
{
}
