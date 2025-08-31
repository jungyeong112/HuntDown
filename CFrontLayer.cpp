#include "pch.h"
#include "CFrontLayer.h"
#include "CBmpMgr.h"

CFrontLayer::CFrontLayer()
{
}

CFrontLayer::~CFrontLayer()
{
}

void CFrontLayer::Initialize()
{
	m_tInfo = { 0.f,0.f, 45.f,100.f };
	
}

int CFrontLayer::Update()
{
	Update_Rect();
	return OBJ_NO_EVENT;
}

void CFrontLayer::LateUpdate()
{
}

void CFrontLayer::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FrontLayer1");

	GdiTransparentBlt(hDC,
		m_tRect.left+38 , m_tRect.top-73 ,
		85, 170.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		0,           //원본 - 복사 시작위치x
		0,          //원본 - 복사 시작위치 y
		45, 100,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                                    //마젠타
}

void CFrontLayer::Release()
{
}
