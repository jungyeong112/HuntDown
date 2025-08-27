#pragma once
#include "CEffect.h"
class CKnockbackDust :public CEffect
{
public:
	CKnockbackDust();
	~CKnockbackDust();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
};

