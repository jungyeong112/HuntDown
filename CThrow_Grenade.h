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
    float m_vx = 0.f;               // 수평 속도
    float m_vy = 0.f;               // 수직 속도
    float m_gravity = 900.f;        // 중력가속도
    float m_restitution = 0.45f;    // 반발계수
    float m_friction = 0.90f;       // 지면 마찰
	float m_fDeathTime = 3.f;
};

