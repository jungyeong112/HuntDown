#pragma once
#include "CGun.h"
class CEnemyUZI: public CGun
{
public:
	CEnemyUZI();
	~CEnemyUZI();

public:

	void Initialize() override;
	void Update() override;
	void Release() override;
	void Fire() override;

};

