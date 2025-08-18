#pragma once
#include "CGun.h"

class CEnemyPistol : public CGun
{
public:
	CEnemyPistol();
	~CEnemyPistol();

public:

	void Initialize() override;
	void Update() override;
	void Release() override;
	void Fire() override;
};

