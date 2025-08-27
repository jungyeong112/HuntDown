#pragma once 
#include "CEffect.h"
class CLandEffect : public CEffect
{
public:
	CLandEffect();
	~CLandEffect();

public:

	// CEffect을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;

};

