#pragma once
#include "CEffect.h"

class CDash_dust : public CEffect
{
public:
	CDash_dust();
	~CDash_dust();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;

};

