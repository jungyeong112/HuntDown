#pragma once
#include "CGun.h"
class CEnemyUZI: public CGun
{
public:
	CEnemyUZI();
	~CEnemyUZI();

public:

	void Initialize() override;
	 int Update() override;
	void Release() override;
	void Fire() override;

};

