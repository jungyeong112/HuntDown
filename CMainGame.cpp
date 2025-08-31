#include "pch.h"
#include "CMainGame.h"
#include "CBmpMgr.h"
#include "CAbstarctFactory.h"
#include "CPlayer.h"
#include "CObjManager.h"
#include "CSceneManager.h"
#include "TimeManager.h"
#include "CKeyMgr.h"
#include "CollisionManager.h"
#include "CUIManager.h"
#include "CSoundMgr.h"
#include "CEffectManager.h"
#include "CScreenManager.h"
#include "CFlagManager.h"



CMainGame::CMainGame() : m_dwTime(GetTickCount()),m_iFPS(0)
{
	
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	m_hDC = GetDC(g_hWnd);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	TimeManager::GetInstance()->Init();
	CSoundMgr::Get_Instance()->Initialize();
	CSceneManager::Get_Instance()->Scene_Change(CSceneManager::SC_MENU);
	CCollisionManager::Instance().Initialize();
	CScreenManager::Instance().Initialize();

	
}

void CMainGame::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	CSceneManager::Get_Instance()->Update();
	TimeManager::GetInstance()->Update();
	CKeyMgr::Get_Instance()->Key_Update();
}

void CMainGame::LateUpdate()
{
	CCollisionManager::Instance().LateUpdate();
	CSceneManager::Get_Instance()->LateUpdate();
}

void CMainGame::Render()
{
	++m_iFPS;
	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	CScreenManager::Instance().Clear();
	auto _hDC = CScreenManager::Instance().GetHDC();
	CSceneManager::Get_Instance()->Render(_hDC);
	CScreenManager::Instance().Present();
	CUIManager::Get_Instance()->Render(_hDC);
}

void CMainGame::Release()
{

	CBmpMgr::Destroy_Instance();
	CObjManager::Destory_Instance();
	CSceneManager::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CUIManager::Destory_Instance();
	CSoundMgr::Destroy_Instance();
	CEffectManager::Destory_Instance();
	CFlagManager::Destory_Instance();
	GdiplusShutdown(gdiplusToken);
	
}
