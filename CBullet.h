#pragma once

#include "CObj.h"


class CBullet : public CObj
{
public:
	CBullet();
	virtual ~CBullet();

public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _pCollision) override;

protected:
	void Check_Die(float fdeltaTime);
private:
	float m_fDeathTime{3.f};
};

