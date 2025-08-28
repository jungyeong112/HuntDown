#pragma once
#include "CShootingEnemy.h"
class CShotgun_Enemy : public CShootingEnemy
{
public:
	CShotgun_Enemy();
	~CShotgun_Enemy();
	void Initialize() override;
	void Render(HDC hDC) override;
};

