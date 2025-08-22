#include "pch.h"
#include "CSubWeapon_Item.h"
#include "CBmpMgr.h"
#include "CUIManager.h"
#include "CItemQuantity.h"
#include "CAbstarctFactory.h"

CSubWeapon_Item::CSubWeapon_Item()
{
}

CSubWeapon_Item::~CSubWeapon_Item()
{
}

void CSubWeapon_Item::Initialize()
{
	Set_Size(50.f, 50.f);
	CUIManager::Get_Instance()->Add_UI(QUANTITY, CAbstractFactory<CItemQuantity>::Create_UI(this, m_iMagazine));
	Set_BodyFrame(0, 7, 0, 200.f);
}

int CSubWeapon_Item::Update()
{
	CObj::Update_Rect();
	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CSubWeapon_Item::LateUpdate()
{
	Move_BodyFrame();
}

void CSubWeapon_Item::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Item_SubWeapon");
	GdiTransparentBlt(hDC,
		m_tRect.left+10, m_tRect.top,
		30.f, 30.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		13 * m_tBodyFrame.iStart,           //원본 - 복사 시작위치x
		16 * m_tBodyFrame.iMotion,          //원본 - 복사 시작위치 y
		13, 16,                                      //복사할 가로 세로 사이즈
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

void CSubWeapon_Item::Release()
{
}
