#include "pch.h"
#include "CMeleeEnemy.h"
#include "TimeManager.h"
#include "CBmpMgr.h"
#include  "CEnemyMelee.h"
#include "CAbstarctFactory.h"
#include "CUIManager.h"
#include "CUI_EnemyHp.h"
#include "CObjManager.h"


CMeleeEnemy::CMeleeEnemy()
{
}

CMeleeEnemy::~CMeleeEnemy()
{
}

void CMeleeEnemy::Initialize()
{
	m_tInfo = { 100.f,200.f , 40.f,70.f };
	m_iMaxHp = m_iCurHp = 7;
	m_fPlayerRange = 500.f;
	m_fSpeed = 200.f;
	m_eCurEnemyState = IDLE;
	m_ObjId = ENEMY;

	m_fKnockbackDistance = 350.f;
	m_fMeleeRange = 30.f;
	OriginCY = m_tInfo.fCY;
	SitCY = OriginCY - 20.f;

	m_vCurVelocity = { 0,0 };
	m_vCurDirection = { 0,0 };
	m_vCurAccerelation = { 0, DGRAVITY };

	Set_Target(CObjManager::Get_Instance()->Get_Player());
	CObj::Update_Rect();

	CUIManager::Get_Instance()->Add_UI(HP_BAR, CAbstractFactory<CUI_EnemyHp>::Create_UI(this));

}

int CMeleeEnemy::Update()
{
	float fDeltatime = TimeManager::GetInstance()->GetDeltaTime();
	ApplyGravity(fDeltatime);
	if (m_eCurEnemyState != DIE && m_eCurEnemyState != KNOCKBACK && !m_bISKickHit)
	{
		Check_Distance(m_pTarget);
	}
	Check_TargetY(m_pTarget);

	if (m_bIsInRange && m_eCurEnemyState != DIE && m_eCurEnemyState != KNOCKBACK)
	{
		m_eCurEnemyState = CHASE;
		Player_Chase(fDeltatime);
	};

	if (m_bIsInRange && m_bIsMelee)
	{
		Melee_Pattern(fDeltatime);
	}
	KnockBack(fDeltatime);

	CObj::UI_ActiveTimer(fDeltatime);
	CObj::Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CMeleeEnemy::LateUpdate()
{
	Change_State();
	Move_LegFrame();
}

void CMeleeEnemy::Render(HDC hDC)
{
	m_pFrameKey = (m_iPlayerDir == +1) ? L"Pistol_Enemy_R" : L"Pistol_Enemy_L";

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left - 15, m_tRect.top - 15,
		65.f, 75.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		42 * m_tLegFrame.iStart,           //원본 - 복사 시작위치x
		42 * m_tLegFrame.iMotion,           //원본 - 복사 시작위치 y
		42, 42,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}
}

void CMeleeEnemy::Release()
{
}
void CMeleeEnemy::OnCollision(FCollision _Collison)
{

	if (_Collison.m_OBJID == GROUND)
	{
		if (_Collison.m_Collisiontype == CF_Bottom)
		{
			Set_CollisionPos(_Collison.m_fY);
		}
		else if (_Collison.m_Collisiontype == CF_Right)
		{
			m_tInfo.fX -= _Collison.m_fX;
		}
		else if (_Collison.m_Collisiontype == CF_Left)
		{
			m_tInfo.fX += _Collison.m_fX;
		}
	}
	if (_Collison.m_OBJID == BOX && !m_bIsJump)
	{
		m_eCurEnemyState = JUMP;
	}

	if (_Collison.m_OBJID == KICK && m_iCurHp > 0)
	{
		if (m_iCurHp > 0)
		{
			--m_iCurHp;
			m_bISKickHit = true;
			m_eCurEnemyState = KNOCKBACK;
			m_bUISetActive = true;
		}
		else
			m_eCurEnemyState = DIE;
		OutputDebugString(L"킥 맞음\n");


	}
	if (_Collison.m_OBJID == FLAT_GROUND)
	{
		if (_Collison.m_Collisiontype == CF_Bottom && !m_bIsMaxJump && m_eCurEnemyState != DIE && m_eCurEnemyState != KNOCKBACK)
		{
			Set_CollisionPos(_Collison.m_fY);
			m_bIsDownJumpable = true;
		}
	}

	if (_Collison.m_OBJID == WALL)
	{
		if (_Collison.m_Collisiontype == CF_Right)
		{
			m_tInfo.fX -= m_tInfo.fCX * 0.5f;
		}
		else if (_Collison.m_Collisiontype == CF_Left)
		{
			m_tInfo.fX += m_tInfo.fCX * 0.5f;
		}
	}
	if (_Collison.m_OBJID == BULLET && !m_bIsHide)
	{
		if (m_iCurHp > 0)
		{
			--m_iCurHp;
			m_bUISetActive = true;
		}
		else
			m_eCurEnemyState = DIE;
	}

	if (_Collison.m_OBJID == HIDE_AREA)
	{
		m_bIsHideArea = true;
	}
	else if (_Collison.m_OBJID != HIDE_AREA)
	{
		m_bIsHideArea = false;
	}
	Set_Pos(m_tInfo.fX, m_tInfo.fY);
	CObj::Update_Rect();
}

void CMeleeEnemy::Change_State()
{
	if (m_eCurEnemyState != m_ePreEnemyState)
	{
		switch (m_eCurEnemyState)
		{
		case CBaseEnemy::IDLE:
			Set_LegFrame(0, 1, 0, 200.f);
			break;
		case CBaseEnemy::CHASE:
			Set_LegFrame(0, 3, 2, 200.f);
			m_bIsChase = true;
			break;
		case CBaseEnemy::SIT_DOWN:
			Set_LegFrame(0, 1, 6, 200.f, false);
			m_tInfo.fCY = SitCY;
			break;
		case CBaseEnemy::TAKE_COVER:
			Set_LegFrame(0, 0, 3, 200.f);
			m_bIsHide = true;
			break;
		case CBaseEnemy::DAMAGE:
			break;
		case CBaseEnemy::JUMP:
			OutputDebugString(L"Jump");
			m_bIsJump = true;
			m_bIsMaxJump = true;
			m_vCurVelocity.fy = -DJUMPSPEED;
			break;


		case CBaseEnemy::KNOCKBACK:
			m_bIsJump = true;
			m_vCurVelocity.fy = -DJUMPSPEED;
			Set_LegFrame(0, 2, 5, 200.f, false);
			break;
		case CBaseEnemy::RELOAD:
			Set_LegFrame(0, 1, 0, 200.f);
			break;
		case CBaseEnemy::MELEE:
			m_bIsMelee = false;
			CreateMelee();
			Set_LegFrame(0, 1, 7, 200.f, false);
			break;
		case CBaseEnemy::DIE:
			m_bIsJump = true;
			m_vCurVelocity.fy = -DJUMPSPEED;
			Set_LegFrame(0, 2, 5, 200.f, false);
			CUIManager::Get_Instance()->Increas_EnemyKill();
			break;
		}
	}
	m_ePreEnemyState = m_eCurEnemyState;
}

void CMeleeEnemy::KnockBack(float fDeltaTime)
{
	if ((m_eCurEnemyState == KNOCKBACK || m_eCurEnemyState == DIE) && m_fKnockBackElapsedTime < m_fKnockBackTime)
	{
		m_fKnockBackElapsedTime += fDeltaTime;
		m_tInfo.fX += (-m_iPlayerDir * m_fKnockbackDistance) * fDeltaTime;
	}
	else if (m_eCurEnemyState == KNOCKBACK && m_fKnockBackElapsedTime >= m_fKnockBackTime)
	{
		m_eCurEnemyState = IDLE;
		m_fKnockBackElapsedTime -= m_fKnockBackTime;
		m_bISKickHit = false;
	}
}

void CMeleeEnemy::Melee_Pattern(float fDeltaTime)
{
	if (m_eCurEnemyState != DIE)
		m_eCurEnemyState = MELEE;
	m_tInfo.fCY = OriginCY;
	m_fPatternElapsedTime += fDeltaTime;
	m_fPatternTime = 0.5f;
	if (m_fPatternElapsedTime >= m_fPatternTime)
	{
		if (m_eCurEnemyState == MELEE)
		{
			m_eCurEnemyState = IDLE;
			m_fPatternTime = 2.f;
		}

		m_fPatternElapsedTime = 0.f;
	}
	if (m_eCurEnemyState != MELEE)
	{
		m_bIsMelee = false;
	}
}

void CMeleeEnemy::CreateMelee()
{
	CObjManager::Get_Instance()->Add_Object(ENEMY_MELEE, CAbstractFactory<CEnemyMelee>::Create(Get_FirePos().fx, Get_FirePos().fy, m_iPlayerDir));
}

