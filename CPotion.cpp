#include "pch.h"
#include "CPotion.h"
#include "CBmpMgr.h"

CPotion::CPotion()
{
}

CPotion::~CPotion()
{
}

void CPotion::Initialize()
{
	m_tInfo = { 100.f,100.f, 22.f,32.f };
	
	m_eTag = ITEM_POTION;
	Set_BodyFrame(0, 3, 0, 100.f);
}

int CPotion::Update()
{
	
	CObj::Update_Rect();
	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CPotion::LateUpdate()
{
	Move_BodyFrame();
}

void CPotion::Render(HDC hdc)
{  
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Potion");

	GdiTransparentBlt(hdc,
		m_tRect.left -20, m_tRect.top -20,
		60.f,  60.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		32.f * m_tBodyFrame.iStart,
		32.f * m_tBodyFrame.iMotion,
		32,32,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                                    //마젠타
	
	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hdc, GetStockObject(WHITE_PEN));

		Rectangle(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);

	}
}

void CPotion::Release()
{
}

void CPotion::OnCollision(FCollision _pCollision)
{
	int curHp = _pCollision.m_pObject->Get_Hp();
	int maxHp = _pCollision.m_pObject->Get_MaxHp();

	if (curHp < maxHp)
	{
		_pCollision.m_pObject->Add_CurHp(1);
		m_bIsDead = true;
	}
}
