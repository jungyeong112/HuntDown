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
}

int CMuzzle_Flash::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ActiveFalseTimer(fDeltaTime);
	Update_Rect();
	return 0;
}

void CMuzzle_Flash::LateUpdate()
{
	Move_LegFrame();
}

void CMuzzle_Flash::Render(HDC hDC)
{
	if (m_bSetActive)
	{
		m_pFrameKey = (m_iPlayerDir == +1) ? L"MuzzleFlash_Pistol" : L"MuzzleFlash_Pistol_L";
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"MuzzelFlash_Pistol");

		GdiTransparentBlt(hDC,
			m_tRect.left, m_tRect.top,
			20, 20,                           //12는 피격 박스와 스프라이트 크기 보정
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
				OutputDebugString((L"\nm_bSetActive : " + std::to_wstring(m_fAngle)).c_str());
			SelectObject(hDC, hOldPen);
			SelectObject(hDC, hOldBrush);
		}
	}
}

void CMuzzle_Flash::Release()
{
}
