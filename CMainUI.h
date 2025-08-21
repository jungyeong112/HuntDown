#pragma once
#include "CUI.h"
class CMainUI :public CUI
{
public:
	CMainUI();
	virtual ~CMainUI();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;

};

