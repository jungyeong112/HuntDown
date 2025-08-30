#include "pch.h"
#include "CAnimation.h"
#include "CBmpMgr.h"
#include "TimeManager.h"
#include "CSoundMgr.h"

CAnimation::CAnimation()
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::Initialize()
{
	m_tInfo = { 350,550,500,500 };
	Set_LegFrame(0, 9, 0, 185.f);
	Set_BodyFrame(0, 11, 0, 210.f);
	CSoundMgr::Get_Instance()->PlaySound(L"IntroFire.WAV", SOUND_EFFECT, 0.8f);
}

int CAnimation::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	Update_Rect();
	CheckDie(fDeltaTime);
	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CAnimation::LateUpdate()
{
	CObj::Move_LegFrame();
	CObj::Move_BodyFrame();
}

void CAnimation::Render(HDC hDC)
{
	HDC mhDC = CBmpMgr::Get_Instance()->Find_Image(L"IntroAnim");
	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		630, 200,                           //12는 피격 박스와 스프라이트 크기 보정
		mhDC,
		286.f * m_tLegFrame.iStart,
		0,
		286.f, 105.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));

	HDC mhTDC = CBmpMgr::Get_Instance()->Find_Image(L"IntroTitle");
	GdiTransparentBlt(hDC,
		m_tRect.left + 100, m_tRect.top - 200,
		400, 100,                           //12는 피격 박스와 스프라이트 크기 보정
		mhTDC,
		286.f * m_tLegFrame.iStart,
		0,
		286.f, 57.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));   //마젠타
}

void CAnimation::Release()
{
}

void CAnimation::CheckDie(float fDeltaTime)
{
	m_fElapsedTime += fDeltaTime;
	if (m_fElapsedTime >= 2.3f)
	{
		m_bIsDead = true;
	}
}
