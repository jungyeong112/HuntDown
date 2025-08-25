#pragma once
#include "CShootingEnemy.h"
class CAK47_Enemy : public CShootingEnemy
{
public:
	CAK47_Enemy();
	~CAK47_Enemy();
	void Initialize() override;
};

