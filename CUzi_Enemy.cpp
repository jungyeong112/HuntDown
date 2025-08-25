#include "pch.h"
#include "CUzi_Enemy.h"
#include "TimeManager.h"
#include  "CBmpMgr.h"
#include "CObjManager.h"
#include "CEnemyUZI.h"
#include  "CEnemyMelee.h"
#include "CAbstarctFactory.h"
#include "CUIManager.h"
#include "CUI_EnemyHp.h"
#include "CPotion.h"

CUzi_Enemy::CUzi_Enemy()
{
}

CUzi_Enemy::~CUzi_Enemy()
{
}

void CUzi_Enemy::Initialize()
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

	m_pEnemyWeapon = make_unique<CEnemyUZI>();
	m_pEnemyWeapon->Initialize();

	Set_Target(CObjManager::Get_Instance()->Get_Player());
	CObj::Update_Rect();

	CUIManager::Get_Instance()->Add_UI(HP_BAR, CAbstractFactory<CUI_EnemyHp>::Create_UI(this));
}
