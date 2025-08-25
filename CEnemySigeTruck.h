#pragma once
#include "CBaseEnemy.h"
class CEnemySigeTruck : public CBaseEnemy

{
public:
	CEnemySigeTruck();
	~CEnemySigeTruck();

public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _Collison) override;
};

