#pragma once
#include "CObj.h"

class CBaseEnemy :public CObj
{
public:
	enum  ENEMYSTATE
	{
		IDLE, CHASE, SIT_DOWN, TAKE_COVER,FIRE, DAMAGE, RELOAD, JUMP , DIE, ENEMYSTATE_END
	};

public:
	CBaseEnemy();
	virtual ~CBaseEnemy();

public:
	void  Check_Distance(CObj* pTarget);
	void  Player_Chase(float fDeltaTime);

protected:
	float m_fDistance{ 0.f }; //플레이어와 거리
	float m_fPlayerRange{ 1000.f }; //플레이어 감지할 범위
	float m_fFind_CoveRange{ 100.f }; //엄폐물 감지 범위
	float m_fPatternTime{ 2.f }; //패턴 변경 시간
	float m_fPatternElapsedTime{ 0.f }; //패턴 누적 시간
	ENEMYSTATE m_eCurEnemyState = ENEMYSTATE_END; //몬스터 상태
	ENEMYSTATE m_ePreEnemyState = ENEMYSTATE_END;
	int m_iEnemySprite = { 1 };  //m_iEnemySprite에 따라서 스프라이트 분기
	bool m_bIsInRange{ false };    //플레이어가 범위 안에 들어왔는지
	bool m_bIsCoverCrouch{ false };  //근처에 앉아서 숨을게 있는지
	bool m_bIsChase{ false };
};

