#pragma once
#include "CObj.h"

class CThrow_Grenade : public CObj
{
public:
	CThrow_Grenade();
	~CThrow_Grenade();

public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _pCollision) override;

public:
	void Check_Die(float fDeltaTime);

private:
	float m_fDeathTime{ 3.f };
	float m_fThrowTime{ 0.f };
	float m_fThorwSpeed{ 1500.f };
};

