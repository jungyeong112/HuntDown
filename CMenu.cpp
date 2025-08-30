#include "pch.h"
#include "CMenu.h"
#include "CAbstarctFactory.h"
#include "CObjManager.h" 
#include "CSceneManager.h"
#include "CBmpMgr.h"
#include "CAnimation.h"
#include "CUIManager.h"
#include "TimeManager.h"
#include "CIntroB.h"

CMenu::CMenu()
{
}

CMenu::~CMenu()
{
	Release();
}

void CMenu::Initialize()
{
	InsertBMP();
	m_eState = INTRO_FIRST;
	CObjManager::Get_Instance()->Add_Object(EFFECT, CAbstractFactory<CAnimation>::Create());
}

void CMenu::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	CObjManager::Get_Instance()->Update();
	if (m_eState == INTRO_SECOND && GetAsyncKeyState(VK_RETURN))
	{
		CObjManager::Get_Instance()->Release();
		CSceneManager::Get_Instance()->Scene_Change(CSceneManager::SC_STAGE);
		return;
	}
	if (GetAsyncKeyState('D'))
		DebugMode = !DebugMode;

	ChangeAnim(fDeltaTime);
}

void CMenu::LateUpdate()
{
	CObjManager::Get_Instance()->LateUpdate();
}

void CMenu::Render(HDC hDC)
{
	if (m_eState == INTRO_FIRST)
		PatBlt(hDC, 0, 0, WINCX, WINCY, BLACKNESS);
	else
	{
		HDC mhDC = CBmpMgr::Get_Instance()->Find_Image(L"IntroBG");
		int srcW = 512;
		int srcH = 272;

		StretchBlt(
			hDC,
			0, 0, WINCX, WINCY,
			mhDC,
			0, 0, srcW, srcH,
			SRCCOPY
		);

	}
	CObjManager::Get_Instance()->Render(hDC);
}

void CMenu::Release()
{

}

void CMenu::InsertBMP()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/IntroAnim.bmp", L"IntroAnim");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/IntroTitle.bmp", L"IntroTitle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/IntroBG.bmp", L"IntroBG");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/IntroTitle_Second.bmp", L"IntroTitle_Seconds");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/IntroAnim_Second.bmp", L"IntroAnim_Second");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/PressAnyButton.bmp", L"PressAnyButton");
}

void CMenu::ChangeAnim(float fDeltaTime)
{
	m_fElapsedTime += fDeltaTime;
	static int i = 0;
	if (m_fElapsedTime >= 2.5f&&!i)
	{
		++i;
		m_eState = INTRO_SECOND;
		CObjManager::Get_Instance()->Add_Object(EFFECT, CAbstractFactory<CIntroB>::Create());
	}
}
