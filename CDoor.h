#pragma once
#include "CObj.h"

class CDoor : public CObj
{
public:
	CDoor();
	~CDoor();

public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;

	void OnCollision(FCollision _pCollision) override;
};

