#pragma once

#include "CObj.h"
class CGround1 : public CObj
{
public:
	CGround1();
	~CGround1();

public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;
	void Release() override;
};

