#include "pch.h"
#include "Weapon_Item.h"
#include "CBmpMgr.h"
#include "CUIManager.h"
#include "CItemQuantity.h"
#include "CAbstarctFactory.h"
Weapon_Item::Weapon_Item()
{
}

Weapon_Item::~Weapon_Item()
{
}

void Weapon_Item::Initialize()
{
	Set_Size(50.f, 50.f);
	CUIManager::Get_Instance()->Add_UI(QUANTITY, CAbstractFactory<CItemQuantity>::Create_UI(this, m_iMagazine));
}

int Weapon_Item::Update()
{
	CObj::Update_Rect();
	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void Weapon_Item::LateUpdate()
{
	Move_BodyFrame();
}

void Weapon_Item::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Item_Weapon");
	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top - 20,
		60.f, 60.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		35 * m_tBodyFrame.iStart,           //원본 - 복사 시작위치x
		32 * m_tBodyFrame.iMotion,          //원본 - 복사 시작위치 y
		35, 32,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                                    //마젠타

	//BOXTYPE * Motion으로 시트에서 출력할거임.
	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

	}
}

void Weapon_Item::Release()
{
}

