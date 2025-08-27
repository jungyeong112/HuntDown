#include "pch.h"
#include "CMuzzle_Flash.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CMuzzle_Flash::CMuzzle_Flash()
{
}

CMuzzle_Flash::~CMuzzle_Flash()
{
}

void CMuzzle_Flash::Initialize()
{
	m_tInfo = { 0,0,20.f,20.f };
	Set_BodyFrame(0, 2, 0, 50.f, false);
	m_pTarget = CObjManager::Get_Instance()->Get_Player();
}

int CMuzzle_Flash::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ActiveFalseTimer(fDeltaTime);
	m_tInfo.fX = m_pTarget->Get_FirePos().fx + 20.f;
	m_tInfo.fY = m_pTarget->Get_FirePos().fy;
	Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CMuzzle_Flash::LateUpdate()
{
	Move_BodyFrame();
	if (m_tBodyFrame.iStart > m_tBodyFrame.iEnd)
	{
		m_bIsDead = true;
	}
}

void CMuzzle_Flash::Render(HDC hDC)
{
	m_pFrameKey = (m_iPlayerDir == +1) ? L"MuzzleFlash_Pistol" : L"MuzzleFlash_Pistol_L";
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	int iOffsetX = (m_iPlayerDir == +1) ? 0 : -62.f;

	GdiTransparentBlt(hDC,
		m_tRect.left + iOffsetX, m_tRect.top - 10,
		50, 50,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		32.f * m_tBodyFrame.iStart,
		0,
		32.f, 32.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                  //마젠타
	OutputDebugString((L"\n ndebu : " + std::to_wstring(m_fAngle)).c_str());

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

void CMuzzle_Flash::Release()
{
}
