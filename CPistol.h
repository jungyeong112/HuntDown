#pragma once
#include "CGun.h"

class CPistol : public CGun
{
public:
	CPistol();
	~CPistol();

public:
	void Initialize() override;
	void Update() override;
	void Release() override;
	void Fire() override;
};

