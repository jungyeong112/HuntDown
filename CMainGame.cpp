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
#include "ScreenManager.h"



CMainGame::CMainGame() : m_dwTime(GetTickCount()),m_iFPS(0)
{

}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	TimeManager::GetInstance()->Init();
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
	CScreenManager::Instance().Update(fDeltaTime);
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

	//HDC		hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	//CSceneManager::Get_Instance()->Render(hBackDC);
	//BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
	CSceneManager::Get_Instance()->Render(_hDC);
	CScreenManager::Instance().Present();
	
}

void CMainGame::Release()
{
	CBmpMgr::Destroy_Instance();
	CObjManager::Destory_Instance();
	CSceneManager::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
}
