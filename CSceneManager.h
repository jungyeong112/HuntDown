#pragma once

#include "CMenu.h"
#include "CStage.h"
class CSceneManager
{
public:
	enum SCENEID
	{
		SC_MENU,
		SC_STAGE,
		SC_BOSS,
		SCENE_END
	};

public:

	CSceneManager();
	~CSceneManager();

public:

	void Scene_Change(SCENEID eID);
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	static CSceneManager* Get_Instance() 
	{
		if (m_pInstance == nullptr) 
		{
			m_pInstance = new CSceneManager;
		}
		return m_pInstance;
	}
	static void Destroy_Instance() 
	{
		if (m_pInstance) 
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CSceneManager* m_pInstance;
	
	CScene*   m_pScene;
	SCENEID   m_eCurScene;
	SCENEID   m_ePreScene;
};

