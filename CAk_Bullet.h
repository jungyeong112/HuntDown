#pragma once
#include "CObj.h"
class CAk_Bullet: public CObj
{
public:
	CAk_Bullet();
	~CAk_Bullet();
public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;
	void Release() override;
	void OnCollision(FCollision _pCollision) override;

private:
	void Check_Die(float fdeltaTime);
private:
	float m_fDeathTime{ 3.f };
};

