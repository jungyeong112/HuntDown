#include "pch.h"
#include "CShootingEnemy.h"
#include "TimeManager.h"
#include  "CBmpMgr.h"
#include "CObjManager.h"
#include "CEnemyPistol.h"

CShootingEnemy::CShootingEnemy()
{
}

CShootingEnemy::~CShootingEnemy()
{
}

void CShootingEnemy::Initialize()
{
	srand(time(NULL));
	m_tInfo = { 100.f,200.f ,40.f,70.f };
	m_iMaxHp = 5;
	m_iCurHp = 5;
	m_fPlayerRange = 500.f;
	m_fSpeed = 200.f;
	m_eCurEnemyState = IDLE;
	m_ObjId = ENEMY;

	OriginCY = m_tInfo.fCY;
	SitCY = OriginCY - 20.f;

	m_vCurVelocity = { 0,0 };
	m_vCurDirection = { 0,0 };
	m_vCurAccerelation = { 0, DGRAVITY };

	m_pEnemyWeapon = make_unique<CEnemyPistol>();
	m_pEnemyWeapon->Initialize();

	Set_BodyFrame(0, 7, 3, 200.f);
	Set_Target(CObjManager::Get_Instance()->Get_Player());
	CObj::Update_Rect();
}

int CShootingEnemy::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ApplyGravity(fDeltaTime);
	Check_Distance(m_pTarget);
	if (m_bIsChase)
		Player_Chase(fDeltaTime);
	Check_Delay(fDeltaTime);
	if (m_bIsInRange && m_iCurHp)
		Select_Pattern(fDeltaTime);
	if(m_bIsChase)
	//CrouchAble_Pattern(fDeltaTime);

	CObj::Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CShootingEnemy::LateUpdate()
{
	Change_State();
	Move_BodyFrame();
}

void CShootingEnemy::Render(HDC hDC)

{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Box_Anim");

	GdiTransparentBlt(hDC,
		m_tRect.left - 15, m_tRect.top - 13,
		65.f, 75.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		32 * m_tBodyFrame.iStart,           //원본 - 복사 시작위치x
		34 * 3,                            //원본 - 복사 시작위치 y
		32, 34,                                      //복사할 가로 세로 사이즈
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

	if (_Collison.m_OBJID == FLAT_GROUND)
	{
		if (_Collison.m_Collisiontype == CF_Bottom && !m_bIsMaxJump)
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
	if (_Collison.m_OBJID == BULLET)
	{
		if (m_iCurHp > 0)
			--m_iCurHp;
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

	OutputDebugString(L"HideAble\n");
}

void CShootingEnemy::Open_Fire_Pattern(float fDeltaTime)
{
	
	m_fPatternElapsedTime += fDeltaTime;
	m_eCurEnemyState = CHASE;
	if (m_fPatternElapsedTime >= m_fPatternTime)
	{
		m_bIsChase = false;
		m_fPatternElapsedTime -= m_fPatternTime;
	}
	
	m_eCurEnemyState = FIRE;
	OutputDebugString(L"Open\n");
}
void CShootingEnemy::CrouchAble_Pattern(float fDeltaTime)
{
	int i = rand() % 2 + 1;
	i % 2 == 0 ? m_eCurEnemyState == FIRE : m_eCurEnemyState == SIT_DOWN;
	wstring str = to_wstring(m_eCurEnemyState) + L"현상태\n";
	OutputDebugString(str.c_str());
	m_fPatternElapsedTime += fDeltaTime;
	if (m_fPatternElapsedTime >= m_fPatternTime) 
	{
		
		if (m_eCurEnemyState == FIRE) 
		{
			m_eCurEnemyState == SIT_DOWN;
		}
		else if(m_eCurEnemyState == SIT_DOWN)
		{
			m_tInfo.fCY = OriginCY;
			m_eCurEnemyState == FIRE;
		}
		m_fPatternElapsedTime -= m_fPatternTime;
	}
	OutputDebugString(L"Crouch\n");
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
			break;
		case CBaseEnemy::CHASE:
			m_bIsChase = true;
			break;
		case CBaseEnemy::SIT_DOWN:
			m_tInfo.fCY = SitCY;
			break;
		case CBaseEnemy::TAKE_COVER:
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
			FireWeapon();
			break;

		case CBaseEnemy::RELOAD:
			break;
		case CBaseEnemy::DIE:
			OutputDebugString(L"사망\n");
			break;
		}
		m_ePreEnemyState = m_eCurEnemyState;
	}
}

void CShootingEnemy::Select_Pattern(float fDeltatime)
{
	if (m_bIsInRange && !m_bIsHideArea && !m_bIsCoverCrouch)
	{
		Open_Fire_Pattern(fDeltatime);
	}
	else if (m_bIsInRange && m_bIsHideArea)
	{
		HideAble_Pattern(fDeltatime);
	}
	else if (m_bIsInRange && m_bIsCoverCrouch && !m_bIsHideArea)
	{
		CrouchAble_Pattern(fDeltatime);
	}

}

void CShootingEnemy::FireWeapon()
{
	if (!m_bIsFire)
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
