#pragma once

#include "CBaseEnemy.h"

class CBossAngel : public CBaseEnemy
{
public:
	CBossAngel();
	~CBossAngel();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _Collison) override;

private:
	void ThrowKnife();
	void Melee();
	void BoxBreak();
	void MeleePattern(float fDeltatime);
	void Change_State();
	void Check_Delay(float fDeltatime);
	void Die_Effect();
	void SelectPattern(float fDeltatime);
	void ChasePattern(float fDeltatime);
	void HideAblePattern(float fDeltatime);


private:
	int  m_iAttackType{ 1 };  //1이 위에서 내려찍기 2가 옆으로 치기
	bool m_bPatternEnd{ true };

};

