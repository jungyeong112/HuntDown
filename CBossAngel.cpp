#include "pch.h"
#include "CBossAngel.h"
#include"CAbstarctFactory.h"
#include "CObjManager.h"
#include "CBossAngel_Knife.h"
#include "CEnemyMelee.h"
#include "TimeManager.h"
#include "CUIManager.h"
#include "CBmpMgr.h"

CBossAngel::CBossAngel()
{
}

CBossAngel::~CBossAngel()
{
}

void CBossAngel::Initialize()
{
	m_tInfo = { 0.f,0.f , 50.f, 100.f };
	m_iMaxHp = m_iCurHp = 50;
	m_fPlayerRange = 500.f;
	m_fSpeed = 300.f;
	m_eCurEnemyState = IDLE;
	m_ObjId = ENEMY;

	m_fShootingRange = 350.f;
	m_fMeleeRange = 50.f;

	OriginCY = m_tInfo.fCY;
	SitCY = OriginCY - 20.f;

	m_vCurVelocity = { 0,0 };
	m_vCurDirection = { 0,0 };
	m_vCurAccerelation = { 0, DGRAVITY };
	m_fPatternTime = 2.f;
	m_fPatternElapsedTime = 0.f;

	Set_Target(CObjManager::Get_Instance()->Get_Player());
	CObj::Update_Rect();

	//CUIManager::Get_Instance()->Add_UI(HP_BAR, CAbstractFactory<CUI_EnemyHp>::Create_UI(this));
}

int CBossAngel::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ApplyGravity(fDeltaTime);
	Check_Distance(m_pTarget);
	SelectPattern(fDeltaTime);
	Check_Delay(fDeltaTime);
	Check_TargetY(m_pTarget);
	if (m_bIsInRange && m_bIsChase)
		Player_Chase(fDeltaTime);
	CObj::Update_Rect();

	return 0;
}

void CBossAngel::LateUpdate()
{
	Change_State();
	Move_LegFrame();
	ThrowKnife();
}

void CBossAngel::Render(HDC hDC)
{
	m_pFrameKey = (m_iPlayerDir == +1) ? L"BossAngel" : L"BossAngel";

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left - 30, m_tRect.top - 60,
		150.f, 150.f,
		hMemDC,
		100 * m_tLegFrame.iStart,           //원본 - 복사 시작위치x
		70 * m_tLegFrame.iMotion,           //원본 - 복사 시작위치 y
		100, 70,                                      //복사할 가로 세로 사이즈
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

void CBossAngel::Release()
{
}

void CBossAngel::OnCollision(FCollision _Collison)
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
			//m_bUISetActive = true;
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

void CBossAngel::ThrowKnife()
{
	if (!m_bIsFire && m_bIsAnimEnd && !m_bPatternEnd)
	{
		CObjManager::Get_Instance()->Add_Object(ENEMYBULLET, CAbstractFactory<CBossAngel_Knife>::Create(m_tInfo.fX, m_tInfo.fY, m_iPlayerDir));
		m_bIsFire = true;
	}
}

void CBossAngel::Melee()
{
	CObjManager::Get_Instance()->Add_Object(ENEMY_MELEE, CAbstractFactory<CEnemyMelee>::Create(Get_FirePos().fx, Get_FirePos().fy, m_iPlayerDir));
}

void CBossAngel::MeleePattern(float fDeltatime)
{
	if (m_eCurEnemyState != DIE)
		m_eCurEnemyState = MELEE;
	m_fPatternElapsedTime += fDeltatime;
	m_fPatternTime = 0.8f;

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

void CBossAngel::Change_State()
{
	if (m_eCurEnemyState != m_ePreEnemyState)
	{
		switch (m_eCurEnemyState)
		{
		case CBaseEnemy::IDLE:
			Set_LegFrame(0, 3, 0, 200.f);
			break;
		case CBaseEnemy::CHASE:
			m_bIsChase = true;
			Set_LegFrame(0, 5, 1, 200.f);
			break;
		case CBaseEnemy::TAKE_COVER:
			Set_LegFrame(0, 0, 6, 200.f);
			m_bIsHide = true;
			break;

		case CBaseEnemy::JUMP:
			Set_LegFrame(0, 4, 2, 200.f);
			OutputDebugString(L"Jump");
			m_bIsJump = true;
			m_bIsMaxJump = true;
			m_vCurVelocity.fy = -DJUMPSPEED;
			break;
		case CBaseEnemy::FIRE:
			m_bThrow = true;
			Set_LegFrame(0, 4, 3, 200.f, false);
			break;

		case CBaseEnemy::MELEE:
			m_bIsMelee = false;
			if (m_iAttackType == 1)
			{
				Set_LegFrame(0, 3, 5, 200.f, false);
				m_iAttackType = 2;
			}
			else
			{
				Set_LegFrame(0, 4, 4, 200.f, false);
				m_iAttackType = 1;
			}
			Melee();
			break;
		case CBaseEnemy::DIE:
			//CreateItem();
			m_bIsJump = true;
			m_vCurVelocity.fy = -DJUMPSPEED;
			Set_LegFrame(0, 2, 5, 200.f, false);
			CUIManager::Get_Instance()->Increas_EnemyKill();
			break;
		}
		m_ePreEnemyState = m_eCurEnemyState;
	}
}

void CBossAngel::Check_Delay(float fDeltatime)
{
	float fDelay = 1.f;
	if (m_bIsFire)
	{
		m_fElapsedTime += fDeltatime;
		if (m_fElapsedTime >= fDelay)
		{
			m_bIsFire = false;
			m_fElapsedTime = 0.f;
		}
	}
}

void CBossAngel::Die_Effect()
{
	if (m_eCurEnemyState = DIE)
		Set_LegFrame(0, 0, 9, 5000.f);
}

void CBossAngel::SelectPattern(float fDeltatime)
{
	if (m_bPatternEnd)
	{
		if (m_bIsMelee)
		{
			MeleePattern(fDeltatime);
		}
		else if (m_bIsHideArea && m_bIsYHeight)
		{
			HideAblePattern(fDeltatime);
		}
		else
			ChasePattern(fDeltatime);
	}


}

void CBossAngel::ChasePattern(float fDeltatime)
{
	if (m_bIsInRange && m_bIsMelee)
	{
		MeleePattern(fDeltatime);
	}
	else if (m_bIsCoverCrouch)
	{
		Set_LegFrame(0, 3, 5, 200.f, false);
		Melee();
	}
	else
	{
		m_eCurEnemyState = CHASE;
	}
}

void CBossAngel::HideAblePattern(float fDeltatime)
{
	if (m_eCurEnemyState == DIE)
		return;

	//if (m_eCurEnemyState != FIRE && m_eCurEnemyState != TAKE_COVER)
	//	m_eCurEnemyState = FIRE;
	m_bIsChase = false;

	if (m_fPatternElapsedTime >= m_fPatternTime)
	{
		if (m_eCurEnemyState == FIRE)
		{
			m_bPatternEnd = false;
			m_eCurEnemyState = TAKE_COVER;
			m_fPatternTime = 2.f;
		}
		else if (m_eCurEnemyState == TAKE_COVER)
		{
			m_eCurEnemyState = FIRE;
			m_fPatternTime = 1.5f;
		}
		m_fPatternElapsedTime = 0.f;
		m_bPatternEnd = true;
	}
	else
	{
		m_fPatternElapsedTime += fDeltatime;
	}
	if (m_eCurEnemyState != TAKE_COVER)
	{
		m_bIsHide = false;
	}
}
