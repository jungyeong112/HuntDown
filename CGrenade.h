#pragma once

#include "CSubWeapon.h" 

class CGrenade : public CSubWeapon
{
public:
	CGrenade();
	~CGrenade();

public:
	// CSubWeapon을(를) 통해 상속됨
	void Initialize() override;
	void Update() override;
	void Release() override;
	void Fire() override;

};

