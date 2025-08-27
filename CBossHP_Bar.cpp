#include "pch.h"
#include "CBossHP_Bar.h"
#include "CBmpMgr.h"
#include "CObj.h"

CBossHP_Bar::CBossHP_Bar()
{
}

CBossHP_Bar::~CBossHP_Bar()
{

}

void CBossHP_Bar::Initialize()
{
	m_tInfo = { 0.f,0.f, 222.f,222.f };
	m_tBgInfo = { 0.f,0.f, 350.f, 70.f };
}

int CBossHP_Bar::Update()
{
	POINT CameraPos = CScreenManager::Instance().GetCamerPos();
	m_tInfo.fX = CameraPos.x + 408.f;
	m_tInfo.fY = CameraPos.y + 145;
	m_tBgInfo.fX = CameraPos.x + 470.f;
	m_tBgInfo.fY = CameraPos.y + 70;
	CUI::Update_Rect();
	Update_BGRect();
	return 0;
}

void CBossHP_Bar::Render(HDC hDC)
{
	int iHp = m_pTarget->Get_Hp();
	//if (!m_bSetActive || iHp <= 0)
	//	return;
	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"BossAngel_HPBg"); //Background 

	GdiTransparentBlt(hDC,
		m_tBgRect.left, m_tBgRect.top,
		350, 35,                           
		hBackDC,
		0,
		0,
		155, 15,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));


	HDC hNameDC = CBmpMgr::Get_Instance()->Find_Image(L"BossAngel_Name");  //Title

	GdiTransparentBlt(hDC,
		m_tBgRect.left, m_tBgRect.top-25,
		300, 25,                           //12는 피격 박스와 스프라이트 크기 보정
		hNameDC,
		0,
		0,
		120, 10,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));


	for (int i = 0; i < iHp; ++i)
	{
		m_tInfo.fX += 4.f;
		Update_Rect();
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_HP");

		GdiTransparentBlt(hDC,
			m_tRect.left, m_tRect.top+3,
			30, 30,                           //12는 피격 박스와 스프라이트 크기 보정
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
		Rectangle(hDC, m_tBgRect.left, m_tBgRect.top, m_tBgRect.right, m_tBgRect.bottom);
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}
}


