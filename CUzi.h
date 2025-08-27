#pragma once
#include "CGun.h"
class CUzi : public CGun
{
public:
	CUzi();
	~CUzi();

public:
	// CGun을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void Release() override;
	void Fire() override;
};

