#pragma once 
#include "CBullet.h"
class CShot_Bullet : public CBullet
{

public:
	CShot_Bullet();
	~CShot_Bullet();

public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;

};

