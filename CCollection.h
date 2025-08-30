#pragma once
#include "CItem.h"
class CCollection:public CItem
{
public:
	CCollection();
	~CCollection();
public:
	// CItem을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _pCollision);
};

