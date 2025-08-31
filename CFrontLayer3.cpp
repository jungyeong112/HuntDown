#include "pch.h"
#include "CFrontLayer3.h"
#include "CBmpMgr.h"

CFrontLayer3::CFrontLayer3()
{
}

CFrontLayer3::~CFrontLayer3()
{
}

void CFrontLayer3::Initialize()
{
	m_tInfo = { 0.f,0.f, 45.f,100.f };
}

void CFrontLayer3::Render(HDC hDC)
{
	HDC hMem1DC = CBmpMgr::Get_Instance()->Find_Image(L"FrontLayer3_1");

	GdiTransparentBlt(hDC,
		m_tRect.left +170 , m_tRect.top -290,
		95, 370.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMem1DC,
		0,           //원본 - 복사 시작위치x
		0,          //원본 - 복사 시작위치 y
	    100, 440,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                                    //마젠타



	HDC hMem2DC = CBmpMgr::Get_Instance()->Find_Image(L"FrontLayer3_2");

	GdiTransparentBlt(hDC,
		m_tRect.left+1630 , m_tRect.top-167 ,
		60, 260.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMem2DC,
		0,           //원본 - 복사 시작위치x
		0,          //원본 - 복사 시작위치 y
		125, 480,                                      //복사할 가로 세로 사이즈
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
