#include "pch.h"
#include "CBossRoom.h"
#include "CBmpMgr.h"

CBossRoom::CBossRoom()
{
}

CBossRoom::~CBossRoom()
{
}

void CBossRoom::Render(HDC hDC)
{
	HDC hMem1DC = CBmpMgr::Get_Instance()->Find_Image(L"BossRoom");
	GdiTransparentBlt(hDC,
		m_tRect.left - 240, m_tRect.top - 122,
		230.f, 140.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMem1DC,
		0,                                      //원본 - 복사 시작위치x
		0,                                       //원본 - 복사 시작위치 y
		90, 60,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));
}
