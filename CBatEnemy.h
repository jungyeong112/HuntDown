#pragma once
#include "CMeleeEnemy.h"

class CBatEnemy :public CMeleeEnemy
{
public:
	CBatEnemy();
	~CBatEnemy();


	void Initialize() override;
	void Render(HDC hDC) override;
};

