#pragma once
#include "CEnemyMelee.h"

class CBoxBreaker : public CEnemyMelee
{
public:
	CBoxBreaker();
	~CBoxBreaker();

public:
	void Initialize() override;
	void OnCollision(FCollision _pCollision) override;
};

