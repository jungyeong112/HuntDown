#pragma once
#include "CObj.h"

class CBaseEnemy :public CObj
{
public:
	enum  ENEMYSTATE
	{
		IDLE, CHASE, MELEE, SIT_DOWN, SIT_DOWN_FIRE, TAKE_COVER, FIRE, DAMAGE, RELOAD, JUMP, KNOCKBACK, DIE, ENEMYSTATE_END
	};

public:
	CBaseEnemy();
	virtual ~CBaseEnemy();

public:
	void  Check_Distance(CObj* pTarget);
	void  Player_Chase(float fDeltaTime);
	void  Check_TargetY(CObj* pTarget);


protected:
	float m_fDistance{ 0.f }; //플레이어와 거리
	float m_fPlayerRange{ 1000.f }; //플레이어 감지할 범위
	float m_fShootingRange{ 100.f }; //총 사거리 범위
	float m_fMeleeRange{ 50.f };    //근접 공격 범위
	float m_fFind_CoveRange{ 100.f }; //엄폐물 감지 범위
	float m_fPatternTime{ 2.f }; //패턴 변경 시간
	float m_fPatternElapsedTime{ 0.f }; //패턴 누적 시간
	float m_fHitTime{ 1.f };            //피격시 UI 띄울 시간
	float m_fHitElapsedTime{ 0.f };
	int m_iChaseY = +1;                  //플레이어 Y체크, 점프할지 다운 점프할지 정할용도

	int m_iEnemySprite = { 1 };  //m_iEnemySprite에 따라서 스프라이트 분기
	bool m_bIsInRange{ false };    //플레이어가 범위 안에 들어왔는지
	bool m_bIsCoverCrouch{ false };  //근처에 앉아서 숨을게 있는지
	bool m_bIsChase{ false };
	bool m_bIsYHeight{ false };   //플레이어 y와 값이 같은 라인일때
	bool m_bIsMelee{ false };     //근접 공격할지 여부
	bool m_bISKickHit{ false };   //플레이어 킥에 맞았는지

	bool  m_bBoxJump = false;       // 이번 점프가 '박스 넘기' 용인지
	bool  m_bBoxJumpLock = false;   // 착지 전까지 재트리거 금지
	float m_fBoxJumpBoost = 250.f;  // 박스 점프 시 X축 이동량
	float m_fLookAhead = 80.f;      // 박스 감지 전방 거리(필요 시 조정)
	
	ENEMYSTATE m_eCurEnemyState = ENEMYSTATE_END; //Enemy 상태
	ENEMYSTATE m_ePreEnemyState = ENEMYSTATE_END;
};

