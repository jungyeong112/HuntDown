#include "pch.h"
#include "CItemQuantity.h"
#include "CBmpMgr.h"
#include "CObj.h"

void CItemQuantity::Initialize()
{
	m_tInfo = { 0,0,20.f,20.f };
	const VECTOR2 vPos = m_pTarget->Get_Pos();
	m_tInfo.fX = vPos.fx;
	m_tInfo.fY = vPos.fy -40;
	Update_Rect();
}

int CItemQuantity::Update()
{
	m_bSetActive = m_pTarget->Get_Dead();
	if (m_bSetActive)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CItemQuantity::LateUpdate()
{
}

void CItemQuantity::Render(HDC hDC)
{
	int iSize = (m_iQuantity >= 100) ? 3 : 2;
	wstring strQuantity[3] = {};
	if (iSize == 2) 
	{
		strQuantity[0] = L"Kill_" + to_wstring((m_iQuantity / 10) % 10);
		strQuantity[1] = L"Kill_" + to_wstring(m_iQuantity % 10);
	}
	else 
	{
		strQuantity[0] = L"Kill_" + to_wstring((m_iQuantity / 100) % 10);
		strQuantity[1] = L"Kill_" + to_wstring((m_iQuantity/10) % 10);
		strQuantity[2] = L"Kill_" + to_wstring(m_iQuantity % 10);
	}
	

	for (int i = 0; i < iSize; ++i)  //숫자 렌더
	{
		HDC hMemNumDC = CBmpMgr::Get_Instance()->Find_Image(strQuantity[i].c_str());
		GdiTransparentBlt(hDC,
			m_tRect.left + (i * 12), m_tRect.top,
			12, 25,
			hMemNumDC,
			0,
			0,
			6, 10,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}
}

void CItemQuantity::Release()
{
}
