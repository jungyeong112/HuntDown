#pragma once 
#include "CObj.h"
class CExplosion : public CObj
{
public:
	CExplosion();
	~CExplosion();

public:

	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	void Check_Die(float fDeltaTime);
	float m_fDeathTime{ 1.0f };
};

