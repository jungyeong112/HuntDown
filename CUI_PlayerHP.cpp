#include "pch.h"
#include "CUI_PlayerHP.h"
#include "CUIManager.h"
#include "CBmpMgr.h"
#include "CScreenManager.h"

CUI_PlayerHP::CUI_PlayerHP()
{
}

CUI_PlayerHP::~CUI_PlayerHP()
{
}

void CUI_PlayerHP::Initialize()
{
	m_tInfo = { 0.f,0.f, 20.f,20.f };
}

int CUI_PlayerHP::Update()
{
	POINT CameraPos = CScreenManager::Instance().GetCamerPos();
	m_tInfo.fX = CameraPos.x +30.f;
	m_tInfo.fY = CameraPos.y+20;
	CUI::Update_Rect();

	return 0;
}

void CUI_PlayerHP::Render(HDC hDC)
{
	int iNum = CUIManager::Get_Instance()->Get_Player()->Get_Hp();
	for (int i = 0; i < iNum; ++i)
	{
		m_tInfo.fX += 20.f;
		Update_Rect();
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Main_PlayerHP");

		GdiTransparentBlt(hDC,
			m_tRect.left+20.f, m_tRect.top,
			35, 20,                           //12는 피격 박스와 스프라이트 크기 보정
			hMemDC,
			0,
			0,
			15, 9,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}

	/*if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));
		for (int i = 0; i < iNum; ++i)
		{
			m_tInfo.fX += 25.f;
			Update_Rect();
			Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		}

		
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}*/
}
