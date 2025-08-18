#pragma once

#include "CBullet.h"

class CThrow_Knife : public CBullet
{
public:
	CThrow_Knife();
	~CThrow_Knife();

public:
	void Initialize() override;
	void Render(HDC hdc) override;
};

