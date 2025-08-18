#pragma once
#include "CItem.h"

class CPotion:public CItem
{
public:
	CPotion();
	~CPotion();

public:
	// CItem을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;
	void Release() override;
	void OnCollision(FCollision _pCollision) override;
};

