#pragma once
#include "CUI.h"
class CItemQuantity :public CUI
{
	// CUI을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
};

