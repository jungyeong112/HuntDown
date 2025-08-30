#pragma once
#include "CAnimation.h"

class CIntroB :public CAnimation
{
public:
	CIntroB();
	~CIntroB();

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

