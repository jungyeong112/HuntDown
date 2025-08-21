#include "pch.h"
#include "CMainUI.h"
#include "ScreenManager.h"
#include "CBmpMgr.h"


CMainUI::CMainUI()
{
}

CMainUI::~CMainUI()
{

}

void CMainUI::Initialize()
{
	m_tInfo = { 0.f,0.f,370.f ,120.f };
}

int CMainUI::Update()
{
	POINT CameraPos = CScreenManager::Instance().GetCamerPos();
	m_tInfo.fX = CameraPos.x + 50;
	m_tInfo.fY = CameraPos.y;
	CUI::Update_Rect();

	return 0;
}

void CMainUI::LateUpdate()
{
}

void CMainUI::Render(HDC hDC)
{

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Main_UI");

	GdiTransparentBlt(hDC,
		m_tRect.left+130, m_tRect.top+60,
		370, 120,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		0,
		0,
		155, 60,                    //복사할 가로 세로 사이즈
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

void CMainUI::Release()
{
}
