#pragma once
#include "CBullet.h"

class CBossAngel_Knife : public CBullet
{
public:
	CBossAngel_Knife();
	~CBossAngel_Knife();

public:
	void Initialize() override;
	void Render(HDC hDC) override;
};

