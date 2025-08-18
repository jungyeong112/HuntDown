#include "pch.h"
#include "CSceneManager.h"

CSceneManager* CSceneManager::m_pInstance = nullptr;

CSceneManager::CSceneManager() : m_pScene(nullptr), m_eCurScene(SC_MENU),m_ePreScene(SC_STAGE)
{
}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Scene_Change(SCENEID eID)
{
	m_eCurScene = eID;
	if (m_eCurScene != m_ePreScene) 
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_MENU:
			m_pScene = new CMenu;
			break;
		case SC_STAGE:
			m_pScene = new CStage;
			break;
		case SC_BOSS:
			break;
		case SCENE_END:
			break;
		default:
			break;
		}
		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}

void CSceneManager::Update()
{
	m_pScene->Update();
}

void CSceneManager::LateUpdate()
{
	m_pScene->LateUpdate();
}

void CSceneManager::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneManager::Release()
{
	Safe_Delete(m_pScene);
}
