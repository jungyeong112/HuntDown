#pragma once

#include "CMainUI.h"

class CUI_PlayerHP :public CMainUI
{
public:
	CUI_PlayerHP();
	~CUI_PlayerHP();

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

