#pragma once
#include "CObjManager.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render();
	void Release();

private:
	HDC m_hDC;

	int		m_iFPS;
	DWORD	m_dwTime;
	TCHAR	m_szFPS[32];
};

