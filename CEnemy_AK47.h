#pragma once
#include "CGun.h"
class CEnemy_AK47 :public CGun
{
public:
	CEnemy_AK47();
	~CEnemy_AK47();
public:

	void Initialize() override;
	int Update() override;
	void Release() override;
	void Fire() override;
};

