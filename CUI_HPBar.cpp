#include "pch.h"
#include "CUI_HPBar.h"
#include "CObj.h"
#include "CBmpMgr.h"

CUI_HPBar::CUI_HPBar()
{
}

CUI_HPBar::~CUI_HPBar()
{
}

void CUI_HPBar::Initialize()
{
	m_tInfo = { 0.f,0.f, 20.f,20.f };
	m_tBgInfo = { 0.f,0.f, 20.f,20.f };
}

int CUI_HPBar::Update()
{
	VECTOR2 targetPos = m_pTarget->Get_Pos();
	m_tInfo.fX = targetPos.fx - 20.f;
	m_tInfo.fY = targetPos.fy - 65.f;
	m_tBgInfo.fX = targetPos.fx - 215.f;
	m_tBgInfo.fY = targetPos.fy - 60.f;
	CUI::Update_Rect();
	Update_BGRect();

	return 0;
	
}

void CUI_HPBar::LateUpdate()
{
}

void CUI_HPBar::Render(HDC hDC)
{

	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"PlayerHP_bg");

	GdiTransparentBlt(hDC,
		m_tBgRECT.left + 210.f, m_tBgRECT.top-10,
		70, 30,                           //12는 피격 박스와 스프라이트 크기 보정
		hBackDC,
		0,
		0,
		30, 10,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));

	int iNum = m_pTarget->Get_Hp();
	for (int i = 0; i < iNum; ++i)
	{
		m_tInfo.fX += 13.f;
		Update_Rect();
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_HP");

		GdiTransparentBlt(hDC,
			m_tRect.left, m_tRect.top,
			18, 20,                           //12는 피격 박스와 스프라이트 크기 보정
			hMemDC,
			0,
			0,
			8, 8,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}

	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));
		for (int i = 0; i < iNum; ++i)
		{
			m_tInfo.fX += 20.f;
			Update_Rect();
			Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		}
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}
}

void CUI_HPBar::Release()
{
}

void CUI_HPBar::Update_BGRect()
{
	m_tBgRECT.left = int(m_tBgInfo.fX - (m_tBgInfo.fCX * 0.5f));
	m_tBgRECT.top = int(m_tBgInfo.fY - (m_tBgInfo.fCY * 0.5f));
	m_tBgRECT.right = int(m_tBgInfo.fX + (m_tBgInfo.fCX * 0.5f));
	m_tBgRECT.bottom = int(m_tBgInfo.fY + (m_tBgInfo.fCY * 0.5f));
}
