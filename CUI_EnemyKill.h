#pragma once

#include "CMainUI.h"

class CUI_EnemyKill :public CMainUI
{
public :
	CUI_EnemyKill();
	~CUI_EnemyKill();

public:

	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
	void Num_UpdateRect();

private:
	INFO m_tNumInfo{};
	RECT m_tNumRect{};
};

