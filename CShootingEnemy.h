#pragma once

#include "CBaseEnemy.h"
#include "CGun.h"

class CShootingEnemy :public CBaseEnemy
{
public:
	CShootingEnemy();
	~CShootingEnemy();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _Collison) override;

public:
	void HideAble_Pattern(float fDeltaTime);
	void Open_Fire_Pattern(float fDeltaTime);
	void CrouchAble_Pattern(float fDeltaTime);
	void Melee_Pattern(float fDeltaTime);
	void Check_Delay(float fDeltaTime);
	void Change_State();
	void Select_Pattern(float fDeltatime);
	void FireWeapon();
	void SelectFire();
	void CreateMelee();
	void KnockBack(float fDeltatime);
	void CreateItem();

protected:
	unique_ptr<CGun> m_pEnemyWeapon{nullptr};
};

