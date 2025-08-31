#pragma once

#include "CBaseEnemy.h"

class CMeleeEnemy : public CBaseEnemy
{
public:
	CMeleeEnemy();
	~CMeleeEnemy();
public:
	enum MELEETYPE
	{
		KNIFE,BAT,TYPE_END
	};

public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _Collison) override;

public:
	void Change_State();
	void KnockBack(float fDeltaTime);
	void Melee_Pattern(float fDeltaTime);
	void CreateMelee();
	void Check_Delay(float fDeltatime);

protected:
	MELEETYPE m_WeaponType{ TYPE_END };
};