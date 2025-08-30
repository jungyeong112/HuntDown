#pragma once
#include "CObj.h"
class CAnimation : public CObj
{
public:
	CAnimation();
	~CAnimation();


	// CObj을(를) 통해 상속됨
	void Initialize() override;

	int Update() override;

	void LateUpdate() override;

	void Render(HDC hDC) override;

	void Release() override;

	void CheckDie(float fDeltaTime);
};

