#pragma once

#include "CGun.h"

class CShotgun :public CGun
{
public:
	CShotgun();
	~CShotgun();

public:
	// CGun을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void Release() override;
	void Fire() override;
};

