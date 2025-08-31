#include "pch.h"
#include "CMainUI.h"
#include "CBmpMgr.h"
#include "CScreenManager.h"


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
	POINT cam = CScreenManager::Instance().GetCamerPos();      // 흔들림 제외 논리 카메라
	POINT shake = CScreenManager::Instance().GetShakeOffset();   // 이번 프레임 셰이크

	// 월드 좌표 = (카메라 + 화면오프셋) - shake 보정
	m_tInfo.fX = cam.x + 50 - shake.x;
	m_tInfo.fY = cam.y + 0 - shake.y;

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
