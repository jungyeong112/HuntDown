#include "pch.h"
#include "CUI_SubSlot.h"
#include"CUIManager.h"
#include "CBmpMgr.h"
#include "CSubWeapon.h"

CUI_SubSlot::CUI_SubSlot()
{
}

CUI_SubSlot::~CUI_SubSlot()
{
}

void CUI_SubSlot::Initialize()
{
	m_tInfo = { 0.f,0.f, 120.f, 50.f };
	m_tNumInfo = { 0.f,0.f, 20.f, 20.f };
}

int CUI_SubSlot::Update()
{
	if (CUIManager::Get_Instance()->Get_Player()->Get_ThrowType() == CSubWeapon::THROWTYPE::KNIFE)
		m_bSetActive = false;
	else
		m_bSetActive = true;

	m_eType = CUIManager::Get_Instance()->Get_Player()->Get_ThrowType();
	m_iQuantity = CUIManager::Get_Instance()->Get_Player()->Get_Sub_Quantity();
	POINT CameraPos = CScreenManager::Instance().GetCamerPos();
	m_tInfo.fX = CameraPos.x + 290.f;
	m_tInfo.fY = CameraPos.y + 15;
	m_tNumInfo.fX = CameraPos.x + 145.f;
	m_tNumInfo.fY = CameraPos.y + 52;
	CUI::Update_Rect();
	Num_UpdateRect();
	return 0;
}

void CUI_SubSlot::Render(HDC hDC)
{
	if (!m_bSetActive) return;


	switch (m_eType)
	{
	case CSubWeapon::THROWTYPE::GRENADE:
		m_pFrameKey = L"UI_GRENADE_Q";
		break;
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey); //보조무기 이미지

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
	    80, 90,
		hMemDC,
		0,
		0,
		50, 50,                          //복사할 가로 세로 사이즈
		RGB(255, 0, 255));

	wstring strQuantity[2] = {};

	if (m_iQuantity < 10)
		strQuantity[0] = L"88";
	else
		strQuantity[0] = to_wstring((m_iQuantity / 10) % 10);
	strQuantity[1] = to_wstring(m_iQuantity % 10);

	for (int i = 0; i < 2; ++i)  //숫자 렌더
	{
		HDC hMemNumDC = CBmpMgr::Get_Instance()->Find_Image(strQuantity[i].c_str());
		m_tNumInfo.fX += 13.f;
		Num_UpdateRect();
		GdiTransparentBlt(hDC,
			m_tNumRect.left + 103, m_tNumRect.top - 15,
			12, 20,
			hMemNumDC,
			0,
			0,
			6, 10,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}

	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));
		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}
}

void CUI_SubSlot::Num_UpdateRect()
{
	m_tNumRect.left = int(m_tNumInfo.fX - (m_tNumInfo.fCX * 0.5f));
	m_tNumRect.top = int(m_tNumInfo.fY - (m_tNumInfo.fCY * 0.5f));
	m_tNumRect.right = int(m_tNumInfo.fX + (m_tNumInfo.fCX * 0.5f));
	m_tNumRect.bottom = int(m_tNumInfo.fY + (m_tNumInfo.fCY * 0.5f));
}
