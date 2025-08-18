#pragma once

#include "CSubWeapon.h"

class CKnife : public CSubWeapon
{
public:
	CKnife();
	~CKnife();

public:

	void Initialize() override;
	void Update() override;
	void Release() override;
	void Fire() override;

};

