#pragma once
#include "CItem.h"
class Weapon_Item : public CItem
{
public:
	Weapon_Item();
	~Weapon_Item();
   
public:
	// CItem을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
};

