#include "pch.h"
#include "CGround1.h"

CGround1::CGround1()
{
}

CGround1::~CGround1()
{
}

void CGround1::Initialize()
{
    
}

int CGround1::Update()
{
    CObj::Update_Rect();
    return OBJ_NO_EVENT;
}

void CGround1::LateUpdate()
{

}

void CGround1::Render(HDC hdc)
{
	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hdc, GetStockObject(WHITE_PEN));

		Rectangle(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);

	}
}

void CGround1::Release()
{
}
