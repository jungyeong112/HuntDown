#pragma once

#include "CBaseEnemy.h"

class CEnemySigeTruck : public CBaseEnemy
{
public:
	CEnemySigeTruck();
	~CEnemySigeTruck();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _Collison) override;

public:
	void Change_State();
	void FireWeapon();
	void SigePattern(float fDeltatime);
	void Check_Delay(float fDeltatime);
	void Chain_Explosion(float fDeltatime);

private:
	int m_iExplosionCount{ 0 };
};

