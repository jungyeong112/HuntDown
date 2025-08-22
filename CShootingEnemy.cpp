#include "pch.h"
#include "CShootingEnemy.h"
#include "TimeManager.h"
#include  "CBmpMgr.h"
#include "CObjManager.h"
#include "CEnemyPistol.h"
#include  "CEnemyMelee.h"
#include "CAbstarctFactory.h"
#include "CUIManager.h"
#include "CUI_EnemyHp.h"
CShootingEnemy::CShootingEnemy()
{
}

CShootingEnemy::~CShootingEnemy()
{
}

void CShootingEnemy::Initialize()
{
	m_tInfo = { 100.f,200.f , 40.f,70.f };
	m_iMaxHp = m_iCurHp = 7;
	m_fPlayerRange = 500.f;
	m_fSpeed = 200.f;
	m_eCurEnemyState = IDLE;
	m_ObjId = ENEMY;


	m_fShootingRange = 350.f;
	m_fKnockbackDistance = 200.f;

	m_fMeleeRange = 30.f;
	OriginCY = m_tInfo.fCY;
	SitCY = OriginCY - 20.f;

	m_vCurVelocity = { 0,0 };
	m_vCurDirection = { 0,0 };
	m_vCurAccerelation = { 0, DGRAVITY };

	m_pEnemyWeapon = make_unique<CEnemyPistol>();
	m_pEnemyWeapon->Initialize();

	Set_Target(CObjManager::Get_Instance()->Get_Player());
	CObj::Update_Rect();

	CUIManager::Get_Instance()->Add_UI(HP_BAR, CAbstractFactory<CUI_EnemyHp>::Create_UI(this));
}

int CShootingEnemy::Update()
{
	if (m_iCurHp <= 0)return OBJ_NO_EVENT;
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ApplyGravity(fDeltaTime);
	if (m_eCurEnemyState != DIE && m_eCurEnemyState != KNOCKBACK && !m_bISKickHit)
	{
		Check_Distance(m_pTarget);
	}
	Check_TargetY(m_pTarget);
	Check_Delay(fDeltaTime);

	if (m_bIsInRange && m_iCurHp && m_eCurEnemyState != KNOCKBACK)
		Select_Pattern(fDeltaTime);

	KnockBack(fDeltaTime);

	if (m_bIsInRange && m_bIsChase && m_eCurEnemyState != DIE && !m_bIsMelee && m_eCurEnemyState != KNOCKBACK)
		Player_Chase(fDeltaTime);

	CObj::UI_ActiveTimer(fDeltaTime);

	CObj::Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CShootingEnemy::LateUpdate()
{
	Change_State();
	Move_LegFrame();
}

void CShootingEnemy::Render(HDC hDC)
{
	if (m_iPlayerDir == +1)
	{
		m_pFrameKey = L"Pistol_Enemy_R";
	}
	else
		m_pFrameKey = L"Pistol_Enemy_L";
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left - 15, m_tRect.top - 15,
		65.f, 75.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		42 * m_tLegFrame.iStart,           //원본 - 복사 시작위치x
		42 * m_tLegFrame.iMotion,           //원본 - 복사 시작위치 y
		42, 42,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                                    //마젠타

	//BOXTYPE * Motion으로 시트에서 출력할거임.
	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
	}
}

void CShootingEnemy::Release()
{
}

void CShootingEnemy::OnCollision(FCollision _Collison)
{

	if (_Collison.m_OBJID == GROUND || _Collison.m_OBJID == BOX)
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
	if (_Collison.m_OBJID == KICK && m_iCurHp > 0)
	{
		if (m_iCurHp > 0)
		{
			--m_iCurHp;
			m_bUISetActive = true;
		}
		else
			m_eCurEnemyState = DIE;
		OutputDebugString(L"킥 맞음\n");
		m_bISKickHit = true;
		m_eCurEnemyState = KNOCKBACK;
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

void CShootingEnemy::HideAble_Pattern(float fDeltaTime)
{
	if (m_eCurEnemyState == DIE)
		return;
	if (m_eCurEnemyState == IDLE || m_eCurEnemyState == CHASE)
		m_eCurEnemyState = FIRE;
	m_bIsChase = false;
	m_fPatternElapsedTime += fDeltaTime;

	if (m_fPatternElapsedTime >= m_fPatternTime)
	{
		if (m_eCurEnemyState == FIRE)
		{
			m_eCurEnemyState = RELOAD;
			m_fPatternTime = 1.f;
		}
		else if (m_eCurEnemyState == RELOAD)
		{
			m_eCurEnemyState = TAKE_COVER;
			m_fPatternTime = 2.f;
		}
		else if (m_eCurEnemyState == TAKE_COVER)
		{
			m_eCurEnemyState = FIRE;
			m_fPatternTime = 1.f;
		}
		m_fPatternElapsedTime = 0.f;
	}
	if (m_eCurEnemyState != TAKE_COVER)
	{
		m_bIsHide = false;
	}
}

void CShootingEnemy::Open_Fire_Pattern(float fDeltaTime)
{
	if (m_eCurEnemyState == DIE)
		return;

	else if (m_eCurEnemyState != CHASE)
		m_eCurEnemyState = FIRE;

	m_fPatternElapsedTime += fDeltaTime;
	if (m_fPatternElapsedTime >= m_fPatternTime)
	{
		if (m_eCurEnemyState == FIRE || m_eCurEnemyState == SIT_DOWN_FIRE)
		{
			m_bIsChase = true;
			m_eCurEnemyState = CHASE;
			m_fPatternTime = 1.5f;
		}
		else if (m_eCurEnemyState == CHASE)
		{
			SelectFire();
			m_fPatternTime = 1.f;
		}

		m_fPatternElapsedTime -= m_fPatternTime;
	}
	if (m_eCurEnemyState != CHASE)
	{
		m_bIsChase = false;
	}
	if (m_eCurEnemyState != SIT_DOWN && m_eCurEnemyState != SIT_DOWN_FIRE)
	{
		m_tInfo.fCY = OriginCY;
	}
}
void CShootingEnemy::CrouchAble_Pattern(float fDeltaTime)
{
	if (m_eCurEnemyState == DIE)
		return;
	if (m_eCurEnemyState != SIT_DOWN)
		m_eCurEnemyState = FIRE;
	m_bIsChase = false;
	m_fPatternElapsedTime += fDeltaTime;

	if (m_fPatternElapsedTime >= m_fPatternTime)
	{
		OutputDebugString(L"Crouch\n");
		if (m_eCurEnemyState == FIRE)
		{
			m_eCurEnemyState = SIT_DOWN;
		}
		else if (m_eCurEnemyState == SIT_DOWN)
		{
			m_eCurEnemyState = FIRE;
		}
		m_fPatternElapsedTime -= m_fPatternTime;
	}
	if (m_eCurEnemyState != SIT_DOWN && m_eCurEnemyState != SIT_DOWN_FIRE)
	{
		m_tInfo.fCY = OriginCY;
	}
}
void CShootingEnemy::Melee_Pattern(float fDeltaTime)
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
void CShootingEnemy::Check_Delay(float fDeltaTime)
{
	float fFireDelay = m_pEnemyWeapon->Get_FireDelay();

	m_fElapsedTime += fDeltaTime;

	if (m_fElapsedTime >= fFireDelay)
	{
		m_bIsFire = false;
		m_fElapsedTime -= fFireDelay;
	}
}
void CShootingEnemy::Change_State()
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
		case CBaseEnemy::FIRE:
			Set_LegFrame(0, 2, 1, 200.f, false);
			FireWeapon();
			break;
		case CBaseEnemy::SIT_DOWN_FIRE:
			Set_LegFrame(0, 3, 4, 200.f, false);
			m_tInfo.fCY = SitCY;
			FireWeapon();
			break;

		case CBaseEnemy::KNOCKBACK:
			m_bIsJump = true;
			m_bIsMaxJump = true;
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
			m_bIsMaxJump = true;
			m_vCurVelocity.fy = -DJUMPSPEED;
			Set_LegFrame(0, 2, 5, 200.f, false);
			CUIManager::Get_Instance()->Increas_EnemyKill();
			break;
		}
		m_ePreEnemyState = m_eCurEnemyState;
	}

}

void CShootingEnemy::Select_Pattern(float fDeltatime)
{
	if (m_bIsInRange && m_bIsMelee)
	{
		Melee_Pattern(fDeltatime);
	}
	else if (m_bIsInRange && !m_bIsHideArea && !m_bIsCoverCrouch)
	{
		Open_Fire_Pattern(fDeltatime);
	}
	else if (m_bIsInRange && m_bIsHideArea)
	{
		HideAble_Pattern(fDeltatime);
	}
	else if (m_bIsInRange && m_bIsCoverCrouch && !m_bIsHideArea)
	{
		m_bIsChase = false;
		CrouchAble_Pattern(fDeltatime);
	}
}

void CShootingEnemy::FireWeapon()
{
	if (!m_bIsFire && m_bIsYHeight)
	{
		m_pEnemyWeapon->Set_FirePos(Get_FirePos(), m_iPlayerDir);

		m_pEnemyWeapon->Fire();
		if (m_pEnemyWeapon->Get_Type() == CGun::GUNTYPE::SHOTGUN)
		{
			m_eCurEnemyState = RELOAD;
		}
		m_bIsFire = true;
	}
}

void CShootingEnemy::SelectFire()
{
	bool isTargetSit = m_pTarget->Get_State_SitDown();
	if (m_bIsYHeight)
	{
		if (isTargetSit)
			m_eCurEnemyState = SIT_DOWN_FIRE;
		else
			m_eCurEnemyState = FIRE;
	}
	else
	{
		if (m_pTarget->Get_Info().fY < m_tInfo.fY)   //플레이어가 위에있을땐 점프해서 따라가게
			m_eCurEnemyState = JUMP;
	}
}

void CShootingEnemy::CreateMelee()
{
	CObjManager::Get_Instance()->Add_Object(ENEMY_MELEE, CAbstractFactory<CEnemyMelee>::Create(Get_FirePos().fx, Get_FirePos().fy, m_iPlayerDir));
}

void CShootingEnemy::KnockBack(float fDeltaTime)
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
