#include "pch.h"
#include "CUI_MainSlot.h"
#include "CUIManager.h"
#include "CBmpMgr.h"
#include "CGun.h"

CUI_MainSlot::CUI_MainSlot()
{
}

CUI_MainSlot::~CUI_MainSlot()
{
}

void CUI_MainSlot::Initialize()
{
	m_tInfo = { 0.f,0.f, 120.f, 50.f };
	m_tNumInfo = { 0.f,0.f, 20.f, 20.f };
}

int CUI_MainSlot::Update()
{
	m_eGunType = CUIManager::Get_Instance()->Get_Player()->Get_PlayerGunType();
	m_iMagazine = CUIManager::Get_Instance()->Get_Player()->Get_Main_Magazine();
	POINT CameraPos = CScreenManager::Instance().GetCamerPos();
	m_tInfo.fX = CameraPos.x + 200.f;
	m_tInfo.fY = CameraPos.y + 42;
	m_tNumInfo.fX = CameraPos.x + 100.f;
	m_tNumInfo.fY = CameraPos.y + 42;
	CUI::Update_Rect(); 
	Num_UpdateRect();
	return 0;
}

void CUI_MainSlot::Render(HDC hDC)
{
	switch (m_eGunType)
	{
	case CGun::GUNTYPE::PISTOL:
		m_pFrameKey = L"UI_PISTOL_B";
		break;
	case CGun::GUNTYPE::UZI:
		m_pFrameKey = L"UI_UZI_B";
		break;
	case CGun::GUNTYPE::SHOTGUN:
		m_pFrameKey = L"UI_SHOTGUN_B";
		break;
	case CGun::GUNTYPE::AK_47:
		m_pFrameKey = L"UI_AK47_B";
		break;
	}

	if (m_iMagazine == 999)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Infinity"); //총알 모양

		GdiTransparentBlt(hDC,
			m_tRect.left+55, m_tRect.top+13,
			30, 25,                           //12는 피격 박스와 스프라이트 크기 보정
			hMemDC,
			0,
			0,
			12, 10,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}
	else 
	{
		int iArray[3] = {};
		if (m_iMagazine < 100)
			iArray[0] = 88;                  //어두운 0 예외처리
		else
			iArray[0] = (m_iMagazine / 100) % 10;
		if (m_iMagazine < 10)
			iArray[1] = 88;
		else
			iArray[1] = (m_iMagazine / 10) % 10;

		iArray[2] = m_iMagazine % 10;

		wstring strMagazine[3];

		for (int i = 0; i < 3; ++i)  //숫자 렌더
		{
			strMagazine[i] = to_wstring(iArray[i]);
			HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(strMagazine[i].c_str());
			m_tNumInfo.fX += 12.f;
			Num_UpdateRect();
			GdiTransparentBlt(hDC,
				m_tNumRect.left + 87, m_tNumRect.top,
				12, 30,                           //12는 피격 박스와 스프라이트 크기 보정
				hMemDC,
				0,
				0,
				6, 15,                    //복사할 가로 세로 사이즈
				RGB(255, 0, 255));
		}
	}


	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey); //총알 모양

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		50, 55,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		0,
		0,
		32, 32,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));

	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));
		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

		for (int i = 0; i < 3; ++i)
		{
			m_tNumInfo.fX += 20.f;
			Num_UpdateRect();

			Rectangle(hDC, m_tNumRect.left, m_tNumRect.top, m_tNumRect.right, m_tNumRect.bottom);
		}
	}

}

void CUI_MainSlot::Num_UpdateRect()
{
	m_tNumRect.left = int(m_tNumInfo.fX - (m_tNumInfo.fCX * 0.5f));
	m_tNumRect.top = int(m_tNumInfo.fY - (m_tNumInfo.fCY * 0.5f));
	m_tNumRect.right = int(m_tNumInfo.fX + (m_tNumInfo.fCX * 0.5f));
	m_tNumRect.bottom = int(m_tNumInfo.fY + (m_tNumInfo.fCY * 0.5f));
}


