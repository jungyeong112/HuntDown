#pragma once
#include "CGun.h"

class CAK_47 : public CGun
{
public:
	CAK_47();
	~CAK_47();

public:

	void Initialize() override;
	void Update() override;
	void Release() override;
	void Fire() override;

};

