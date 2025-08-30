#include "pch.h"
#include "CCollection.h"
#include "CBmpMgr.h"
#include "CUIManager.h"
CCollection::CCollection()
{
}

CCollection::~CCollection()
{
}

void CCollection::Initialize()
{
	Set_Size(50.f, 50.f);
	Set_BodyFrame(0, 3, 0, 200.f);
}

int CCollection::Update()
{
	CObj::Update_Rect();
	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CCollection::LateUpdate()
{
	Move_BodyFrame();
}

void CCollection::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Item_Collection");
	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top - 20,
		60.f, 60.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		32 * m_tBodyFrame.iStart,           //원본 - 복사 시작위치x
		32 * m_tBodyFrame.iMotion,          //원본 - 복사 시작위치 y
		32, 32,                                      //복사할 가로 세로 사이즈
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

void CCollection::Release()
{
}

void CCollection::OnCollision(FCollision _pCollision)
{
	if (_pCollision.m_OBJID == PLAYER)
	{
		CUIManager::Get_Instance()->Increase_Collection();
		m_bIsDead = true;
	}
}
