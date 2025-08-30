#include "pch.h"
#include "CUI_EnemyKill.h"
#include "CUIManager.h"
#include "CBmpMgr.h"
#include "CScreenManager.h"

CUI_EnemyKill::CUI_EnemyKill()
{
}

CUI_EnemyKill::~CUI_EnemyKill()
{
}

void CUI_EnemyKill::Initialize()
{
	m_tInfo = { 0.f,0.f, 70.f, 50.f };
	m_tNumInfo = { 0.f,0.f, 20.f, 20.f };
}

int CUI_EnemyKill::Update()
{
	POINT CameraPos = CScreenManager::Instance().GetCamerPos();
	m_tInfo.fX = CameraPos.x + 215.f;
	m_tInfo.fY = CameraPos.y + 32;
	m_tNumInfo.fX = CameraPos.x + 100.f;
	m_tNumInfo.fY = CameraPos.y + 32;
	Update_Rect();
	Num_UpdateRect();
	return 0;
}

void CUI_EnemyKill::Render(HDC hDC)
{
	int iEnemyKill = CUIManager::Get_Instance()->Get_EnemyKill();

	wstring strKill[2] = {};

	if (iEnemyKill < 10)
		strKill[0] = L"Dark_0";
	else
	strKill[0] = L"Kill_" + to_wstring((iEnemyKill / 10) % 10);
	strKill[1] = L"Kill_" + to_wstring(iEnemyKill % 10);

	for (int i = 0; i < 2; ++i)  //숫자 렌더
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(strKill[i].c_str());
		m_tNumInfo.fX += 13.f;
		Num_UpdateRect();
		GdiTransparentBlt(hDC,
			m_tNumRect.left + 103, m_tNumRect.top-15,
			12, 25,                           //12는 피격 박스와 스프라이트 크기 보정
			hMemDC,
			0,
			0,
			6, 10,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Main_Skul"); //해골

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		23, 23,                          
		hMemDC,
		0,
		0,
		10, 10,                          //복사할 가로 세로 사이즈
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

void CUI_EnemyKill::Num_UpdateRect()
{
	m_tNumRect.left = int(m_tNumInfo.fX - (m_tNumInfo.fCX * 0.5f));
	m_tNumRect.top = int(m_tNumInfo.fY - (m_tNumInfo.fCY * 0.5f));
	m_tNumRect.right = int(m_tNumInfo.fX + (m_tNumInfo.fCX * 0.5f));
	m_tNumRect.bottom = int(m_tNumInfo.fY + (m_tNumInfo.fCY * 0.5f));
}
