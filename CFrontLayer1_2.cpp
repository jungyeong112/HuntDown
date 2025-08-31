#include "pch.h"
#include "CFrontLayer1_2.h"
#include "CBmpMgr.h"

CFrontLayer1_2::CFrontLayer1_2()
{
}

CFrontLayer1_2::~CFrontLayer1_2()
{
}

void CFrontLayer1_2::Render(HDC hDC)
{
	HDC hMem1DC = CBmpMgr::Get_Instance()->Find_Image(L"FrontLayer2");

	GdiTransparentBlt(hDC,
		m_tRect.left -93, m_tRect.top - 278,
		45, 170.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMem1DC,
		0,           //원본 - 복사 시작위치x
		0,          //원본 - 복사 시작위치 y
		30, 105,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                                    //마젠타



	HDC hMem2DC = CBmpMgr::Get_Instance()->Find_Image(L"FrontLayer3");

	GdiTransparentBlt(hDC,
		m_tRect.left +295, m_tRect.top-129,
		40, 160.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMem2DC,
		0,           //원본 - 복사 시작위치x
		0,          //원본 - 복사 시작위치 y
		25, 90,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));
	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

	}
}
