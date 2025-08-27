#pragma once
#include "CUI_EnemyHp.h"
class CBossHP_Bar :public CUI_EnemyHp
{
public:
	CBossHP_Bar();
	~CBossHP_Bar();

	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
}; 

