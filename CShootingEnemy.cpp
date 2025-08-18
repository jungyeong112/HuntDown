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
	m_tInfo = { 100.f,200.f ,40.f,70.f };
	m_iMaxHp = 5;
	m_iCurHp = 5;
	m_fPlayerRange = 500.f;
	m_fSpeed = 200.f;
	m_eCurEnemyState = IDLE;
	m_ObjId = ENEMY;

	m_pEnemyWeapon = make_unique<CEnemyPistol>();
	m_pEnemyWeapon->Initialize();

	Set_BodyFrame(0, 7, 3, 200.f);
	Set_Target(CObjManager::Get_Instance()->Get_Player());
	Update_Rect();
}

int CShootingEnemy::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

	Check_Distance(m_pTarget);
	Check_Delay(fDeltaTime);
	if (m_bIsChase && m_iCurHp)
		Select_Pattern(fDeltaTime);
	Change_State();
	CObj::Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CShootingEnemy::LateUpdate()
{
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
}

void CShootingEnemy::HideAble_Pattern(float fDeltaTime)
{

	OutputDebugString(L"HideAble\n");
}

void CShootingEnemy::Open_Fire_Pattern(float fDeltaTime)
{
	m_fPatternElapsedTime += fDeltaTime;
	if (m_fPatternElapsedTime >= m_fPatternTime)
	{
		m_eCurEnemyState = FIRE;
		m_fPatternElapsedTime -= m_fPatternTime;
	}
	else m_eCurEnemyState = CHASE;
	OutputDebugString(L"Open\n");
}
void CShootingEnemy::CrouchAble_Pattern(float fDeltaTime)
{
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
			break;
		case CBaseEnemy::SIT_DOWN:

			break;
		case CBaseEnemy::TAKE_COVER:
			break;
		case CBaseEnemy::DAMAGE:
			break;

		case CBaseEnemy::FIRE:
			FireWeapon();
			break;

		case CBaseEnemy::RELOAD:
			break;
		case CBaseEnemy::DIE:
			OutputDebugString(L"사망\n");
			break;
		case CBaseEnemy::ENEMYSTATE_END:
			break;
		default:
			m_eCurEnemyState = IDLE;
			break;
		}
		m_ePreEnemyState = m_eCurEnemyState;
	}
}

void CShootingEnemy::Select_Pattern(float fDeltatime)
{
	if (m_bIsChase && !m_bIsHideArea && !m_bIsCoverCrouch)
	{
		Open_Fire_Pattern(fDeltatime);
	}
	else if (m_bIsChase && m_bIsHideArea)
	{
		HideAble_Pattern(fDeltatime);
	}
	else if (m_bIsChase && m_bIsCoverCrouch && !m_bIsHideArea)
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
