#pragma once
#include "CGun.h"
class CEnemy_Shotgun :public CGun
{
public:
	CEnemy_Shotgun();
	~CEnemy_Shotgun();

public:


	// CGun을(를) 통해 상속됨
	void Initialize() override;

	void Update() override;

	void Release() override;

	void Fire() override;

};

