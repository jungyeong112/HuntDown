#pragma once
#include "CObj.h"

class CShot_BulletEffect : public CObj
{
public:
	CShot_BulletEffect();
	~CShot_BulletEffect();

public:

	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _pCollision) override;
};

