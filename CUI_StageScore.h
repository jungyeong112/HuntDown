#pragma once
#include "CUI.h"
class CUI_StageScore :public CUI
{
public:
	CUI_StageScore();
	~CUI_StageScore();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;



};

