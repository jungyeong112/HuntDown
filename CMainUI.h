#pragma once
#include "CUI.h"
class CMainUI :public CUI
{
public:
	CMainUI();
	~CMainUI();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;

};

