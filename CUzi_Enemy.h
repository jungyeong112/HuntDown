#pragma once

#include "CShootingEnemy.h"

class CUzi_Enemy :public CShootingEnemy
{
public:
	CUzi_Enemy();
	~CUzi_Enemy();
	void Initialize() override;
};

