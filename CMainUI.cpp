#include "pch.h"
#include "CMainUI.h"
#include "ScreenManager.h"


CMainUI::CMainUI()
{
}

CMainUI::~CMainUI()
{

}

void CMainUI::Initialize()
{
	m_tInfo = { 0.f,0.f,300.f ,100.f };
}

int CMainUI::Update()
{
	POINT CameraPos = CScreenManager::Instance().GetCamerPos();
	m_tInfo.fX = CameraPos.x;
	m_tInfo.fY = CameraPos.y;
	CUI::Update_Rect();

	return 0;
}

void CMainUI::LateUpdate()
{
}

void CMainUI::Render(HDC hDC)
{
	if (DebugMode)
	{
		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	}
}

void CMainUI::Release()
{
}
