#include "pch.h"
#include "CThrow_Knife.h"
#include "CBmpMgr.h"

CThrow_Knife::CThrow_Knife()
{
}

CThrow_Knife::~CThrow_Knife()
{
}

void CThrow_Knife::Initialize()
{
	m_tInfo = { 100.f, 100.f, 15.f, 15.f };
	m_fSpeed = 1000.f;
	Set_BodyFrame(0, 0, 0, 100.f);
}

void CThrow_Knife::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Throw_Knife");

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		42, 40,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		32.f * m_tBodyFrame.iStart,
		32.f * m_tBodyFrame.iMotion,
		32.f, 32.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                  //마젠타

	if (DebugMode)
	{
		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	}
}
