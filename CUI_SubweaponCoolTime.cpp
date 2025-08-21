#include "pch.h"
#include "CUI_SubweaponCoolTime.h"
#include "CBmpMgr.h"
#include "CUIManager.h"
#include "CSubWeapon.h"

CUI_SubweaponCoolTime::CUI_SubweaponCoolTime()
{
}

CUI_SubweaponCoolTime::~CUI_SubweaponCoolTime()
{
}

void CUI_SubweaponCoolTime::Initialize()
{
	m_tInfo = { 0.f,0.f, 50.f,120.f };
}

int CUI_SubweaponCoolTime::Update()
{
	if (CUIManager::Get_Instance()->Get_Player()->Get_ThrowType() == CSubWeapon::THROWTYPE::KNIFE)
		m_bSetActive = true;
	else
		m_bSetActive = false;

	POINT CameraPos = CScreenManager::Instance().GetCamerPos();
	m_tInfo.fX = CameraPos.x + 270.f;
	m_tInfo.fY = CameraPos.y;


	float fKnifeCoolTime = 3.f;
	float fCoolElapsedTime = CUIManager::Get_Instance()->Get_Player()->Get_SubWeaponElapsedTime();
	float fRatio = clamp(fCoolElapsedTime / fKnifeCoolTime, 0.f, 1.f);

	m_fCoolRatio = fRatio;

	CUI::Update_Rect();
	return 0;
}

void CUI_SubweaponCoolTime::Render(HDC hDC)
{
    if (!m_bSetActive) return;

    const int baseX = (int)lround(m_tRect.left - 15.0);
    const int baseY = (int)lround(m_tRect.top + 65.0);

    HDC hBg = CBmpMgr::Get_Instance()->Find_Image(L"UI_Knife_bg");
    HDC hKnife = CBmpMgr::Get_Instance()->Find_Image(L"UI_Knife");

    GdiTransparentBlt(hDC, baseX, baseY, 70, 70, hBg, 0, 0, 32, 32, RGB(255, 0, 255));

    // 칼 아이콘도 “항상 전체 70x70”로 그리지만, 클리핑으로 아래쪽 일부만 보이게
    const double ratio = std::clamp(m_fCoolRatio, 0.f, 1.f);
    const int show_h = (int)lround(70.0 * ratio);  // 화면에 보일 높이
    const int show_y = 70 - show_h;                // 아래서부터 차오르게

    if (show_h > 0) {
        const int saved = SaveDC(hDC);
        // 보이는 영역만 클립 (화면 좌표)
        IntersectClipRect(hDC,
            baseX, baseY + show_y,
            baseX + 70, baseY + 70);

        // 전체를 그리되 클립으로 잘리게 함
        GdiTransparentBlt(hDC,
            baseX, baseY,
            70, 70,
            hKnife,
            0, 0,
            32, 32,
            RGB(255, 0, 255));
        RestoreDC(hDC, saved);
    }
    if (DebugMode) {
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
        HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));
        Rectangle(hDC, baseX, baseY + show_y, baseX + 70, baseY + 70);
        SelectObject(hDC, hOldPen);
        SelectObject(hDC, hOldBrush);
    }
}




