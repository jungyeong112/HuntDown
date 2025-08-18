#pragma once
#include "CObj.h"

class CBaseEnemy :public CObj
{
	enum  ENEMYSTATE
	{
		IDLE, PATROLL, ATTACK, TAKE_COVER, DAMAGE ,RELOAD ,DIE , ENEMYSTATE_END
	};

public:
	CBaseEnemy();
	virtual ~CBaseEnemy();

public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
};

