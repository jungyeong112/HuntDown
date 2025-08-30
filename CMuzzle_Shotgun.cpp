#include "pch.h"
#include "CMuzzle_Shotgun.h"
#include "TimeManager.h"
#include "CBmpMgr.h"

CMuzzle_Shotgun::CMuzzle_Shotgun()
{
}

CMuzzle_Shotgun::~CMuzzle_Shotgun()
{
}

void CMuzzle_Shotgun::Initialize()
{
	m_tInfo = { 0,0,20.f,25.f };
	Set_BodyFrame(0, 4, 0, 50.f, false);
}

int CMuzzle_Shotgun::Update()
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

void CMuzzle_Shotgun::Render(HDC hDC)
{
	m_pFrameKey = (m_iPlayerDir == +1) ? L"ShotMuzzle" : L"ShotMuzzle_L";
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	int iOffsetX = (m_iPlayerDir == +1) ? 0 : -57;

	GdiTransparentBlt(hDC,
		m_tRect.left + iOffsetX, m_tRect.top,
		50, 50,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		35.f * m_tBodyFrame.iStart,
		0,
		35.f, 35.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                  //마젠타

	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));
		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		OutputDebugString((L"\nm_bSetActive : " + std::to_wstring(m_fAngle)).c_str());
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}
}
