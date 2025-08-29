#include "pch.h"
#include "CSiegeTruck_HpBar.h"
#include "CBmpMgr.h"

CSiegeTruck_HpBar::CSiegeTruck_HpBar()
{
}

CSiegeTruck_HpBar::~CSiegeTruck_HpBar()
{
}

void CSiegeTruck_HpBar::Initialize()
{
  
}

int CSiegeTruck_HpBar::Update()
{
	static int iMaxHP = m_pTarget->Get_MaxHp();
	float iCurHp = m_pTarget->Get_Hp();
    m_tInfo = m_tBgInfo = m_pTarget->Get_Info();
    Update_Rect();
	m_fRatio = std::clamp(iCurHp / iMaxHP, 0.f, 1.f);
 
	return 0;
}

void CSiegeTruck_HpBar::LateUpdate()
{
}

void CSiegeTruck_HpBar::Render(HDC hDC)
{
    bool bRender = m_pTarget->Get_UIActive();
    int CurHp = m_pTarget->Get_Hp();
    if (bRender && CurHp) 
    {
        HDC hBg = CBmpMgr::Get_Instance()->Find_Image(L"SiegeTruckHpBg");
        GdiTransparentBlt(hDC,
            m_tRect.left, m_tRect.top - 53,
            200, 30,
            hBg,
            0, 0,
            65, 8,
            RGB(255, 0, 255));

        HDC hpdc = CBmpMgr::Get_Instance()->Find_Image(L"SiegeTruckHp");
        GdiTransparentBlt(hDC,
            m_tRect.left, m_tRect.top - 53,
            (200 * m_fRatio), 30,
            hpdc,
            0, 0,
            (65 * m_fRatio), 8,
            RGB(255, 0, 255));
    }
   
}

void CSiegeTruck_HpBar::Release()
{
}

void CSiegeTruck_HpBar::Update_BGRect()
{
}
