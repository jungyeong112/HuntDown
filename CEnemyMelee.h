#pragma once

#include "CObj.h"

class CEnemyMelee : public CObj
{
public:

	CEnemyMelee();
	~CEnemyMelee();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _pCollision) override;

private:

	void Check_Die(float fDeltaTime);

private:

	float m_fDeathTime{ 0.5f };
};

