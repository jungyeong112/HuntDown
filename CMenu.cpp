#include "pch.h"
#include "CMenu.h"
#include "CAbstarctFactory.h"
#include "CObjManager.h" 
#include "CSceneManager.h"

CMenu::CMenu()
{
}

CMenu::~CMenu()
{
	Release();
}

void CMenu::Initialize()
{
	MessageBox(NULL, _T("메뉴 화면"), _T("CMenu"), MB_OK);
}

void CMenu::Update()
{
	if (GetAsyncKeyState(VK_RETURN)) 
	{
		CSceneManager::Get_Instance()->Scene_Change(CSceneManager::SC_STAGE);
		return;
	}
}

void CMenu::LateUpdate()
{
	
}

void CMenu::Render(HDC hDC) 
{
	
}

void CMenu::Release()
{

}
