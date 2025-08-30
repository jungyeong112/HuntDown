#include "pch.h"
#include "CIntroB.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"
CIntroB::CIntroB()
{
}

CIntroB::~CIntroB()
{
}

void CIntroB::Initialize()
{
	m_tInfo = { 350,550,500,500 };
	Set_LegFrame(0, 5, 0, 200.f);
	Set_BodyFrame(0, 10, 0, 210.f);
	CSoundMgr::Get_Instance()->PlayBGM(L"MainMenu_Loop.wav", 0.8f);
	CSoundMgr::Get_Instance()->PlaySound(L"IntroVoice.WAV",SOUND_EFFECT, 0.8f);
}

int CIntroB::Update()
{
	Update_Rect();
	return 0;
}

void CIntroB::Render(HDC hDC)
{
	HDC mhDC = CBmpMgr::Get_Instance()->Find_Image(L"IntroAnim_Second");
	GdiTransparentBlt(hDC,
		m_tRect.left+150, m_tRect.top,
		250, 130,                           //12는 피격 박스와 스프라이트 크기 보정
		mhDC,
		74.f * m_tLegFrame.iStart,
		0,
		74.f, 58.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));

	HDC mhTDC = CBmpMgr::Get_Instance()->Find_Image(L"IntroTitle_Seconds");
	GdiTransparentBlt(hDC,
		m_tRect.left + 100, m_tRect.top - 200,
		400, 100,                           //12는 피격 박스와 스프라이트 크기 보정
		mhTDC,
		286.f * m_tLegFrame.iStart,
		0,
		286.f, 57.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));   //마젠타

	DWORD CurTime = GetTickCount64();
	if ((CurTime / 500) % 2) 
	{
		HDC mbtDC = CBmpMgr::Get_Instance()->Find_Image(L"PressAnyButton");
		GdiTransparentBlt(hDC,
			m_tRect.left + 130, m_tRect.top + 180,
			300, 20,                           //12는 피격 박스와 스프라이트 크기 보정
			mbtDC,
			0,
			0,
			124.f, 8.f,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));   //마젠타
	}

}
