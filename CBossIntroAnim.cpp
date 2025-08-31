#include "pch.h"
#include "CBossIntroAnim.h"
#include "CBmpMgr.h"

CBossIntroAnim::CBossIntroAnim()
{
}

CBossIntroAnim::~CBossIntroAnim()
{
}

void CBossIntroAnim::Initialize()
{
	Set_Size(50.f, 50.f);
	Set_LegFrame(0, 4, 0, 200.f,false);
}

int CBossIntroAnim::Update()
{
	CObj::Update_Rect();
	return 0;
}

void CBossIntroAnim::LateUpdate()
{
	Move_LegFrame();
}

void CBossIntroAnim::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BossIntroAnim");
	GdiTransparentBlt(hDC,
		m_tRect.left+153, m_tRect.top - 115,
		270.f, 155.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		110 * m_tLegFrame.iStart,           //원본 - 복사 시작위치x
		0,         //원본 - 복사 시작위치 y
		110	, 70,                                      //복사할 가로 세로 사이즈
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

void CBossIntroAnim::Release()
{
}
