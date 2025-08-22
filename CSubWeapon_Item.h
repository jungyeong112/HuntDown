#pragma once
#include "CItem.h"
class CSubWeapon_Item : public CItem
{
public:
	CSubWeapon_Item();
	~CSubWeapon_Item();
public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
};

