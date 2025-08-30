#include"pch.h"
#include "CEnemySigeTruck.h"
#include "CAbstarctFactory.h"
#include "CUIManager.h"
#include  "CSiegeTruck_HpBar.h"
#include "TimeManager.h"
#include  "CBmpMgr.h"
#include "CObjManager.h"
#include "CAk_Bullet.h"
#include "CExplosion.h"
#include "CSoundMgr.h"
#include "CBullet.h"
#include "CEffectManager.h"
#include "CMuzzle_Machine.h"
#include "CMuzzle_Siege.h"
#include "CScreenManager.h"

CEnemySigeTruck::CEnemySigeTruck()
{

}

CEnemySigeTruck::~CEnemySigeTruck()
{

}

void CEnemySigeTruck::Initialize()
{
	m_tInfo = { 0,0, 100.f,200.f };
	m_iMaxHp = m_iCurHp = 50;
	m_ObjId = ENEMY;
	m_eCurEnemyState = IDLE;
	m_fShootingRange = 350.f;

	Set_Target(CObjManager::Get_Instance()->Get_Player());
	CObj::Update_Rect();

	CUIManager::Get_Instance()->Add_UI(HP_BAR, CAbstractFactory<CSiegeTruck_HpBar>::Create_UI(this));
	Set_LegFrame(0, 3, 0, 200.f);
}

int CEnemySigeTruck::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	Check_Delay(fDeltaTime);
	Check_Distance(m_pTarget);
	SigePattern(fDeltaTime);
	Chain_Explosion(fDeltaTime);
	CObj::UI_ActiveTimer(fDeltaTime);
	CObj::Update_Rect();
	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CEnemySigeTruck::LateUpdate()
{
	Change_State();
	Move_LegFrame();
}

void CEnemySigeTruck::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"EnemySiegeTruck");

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top - 150,
		300.f, 300.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		200 * m_tLegFrame.iStart,           //원본 - 복사 시작위치x
		200 * m_tLegFrame.iMotion,           //원본 - 복사 시작위치 y
		200, 200,                                      //복사할 가로 세로 사이즈
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

void CEnemySigeTruck::Release()
{
}

void CEnemySigeTruck::OnCollision(FCollision _Collison)
{
	if (_Collison.m_OBJID == BULLET)
	{
		if (m_iCurHp > 0)
		{
			--m_iCurHp;
			m_bUISetActive = true;
			m_bIsHit = true;
		}
		else
			m_eCurEnemyState = DIE;
	}
	m_bIsHit = false;
}

void CEnemySigeTruck::Change_State()
{
	if (m_eCurEnemyState != m_ePreEnemyState)
	{
		switch (m_eCurEnemyState)
		{
		case CBaseEnemy::IDLE:
			Set_LegFrame(0, 3, 0, 200.f);
			break;

		case CBaseEnemy::FIRE:
			Set_LegFrame(0, 2, 1, 200.f, false);
			FireWeapon();
			break;

		case CBaseEnemy::DIE:
			Set_LegFrame(0, 0, 2, 200.f, false);
			CUIManager::Get_Instance()->Increas_EnemyKill();
			break;
		}
		m_ePreEnemyState = m_eCurEnemyState;

	}
	else if (m_eCurEnemyState == FIRE && m_iCurHp > 0 && !m_bIsFire)
	{
		FireWeapon();
	}
}

void CEnemySigeTruck::FireWeapon()
{
	if (!m_bIsFire)
	{
		CEffectManager::Get_Instance()->Add_EFFECT(MUZZLE_FLASH, CAbstractFactory<CMuzzle_Siege>::CreateEffect(m_tInfo.fX , m_tInfo.fY + 47.f,-1,this));
		CEffectManager::Get_Instance()->Add_EFFECT(MUZZLE_FLASH, CAbstractFactory<CMuzzle_Machine>::CreateEffect(m_tInfo.fX, m_tInfo.fY, -1, this));
		CSoundMgr::Get_Instance()->PlaySound(L"AK47End.wav", ENEMY_FIRE, 0.9f);
		CObjManager::Get_Instance()->Add_Object(ENEMYBULLET, CAbstractFactory<CAk_Bullet>::Create(m_tInfo.fX - 90.f, m_tInfo.fY + 47.f, -1, 600));
		CObjManager::Get_Instance()->Add_Object(ENEMYBULLET, CAbstractFactory<CBullet>::Create(m_tInfo.fX - 5.f, m_tInfo.fY - 30.f, -1, 600));
		m_bIsFire = true;
	}

}

void CEnemySigeTruck::SigePattern(float fDeltatime)
{
	if (m_eCurEnemyState == DIE)
		return;

	if (m_fPatternElapsedTime >= m_fPatternTime)
	{

		m_eCurEnemyState = (m_eCurEnemyState == IDLE) ? FIRE : IDLE;
		m_fPatternElapsedTime -= m_fPatternTime;

	}
	else
	{
		m_fPatternElapsedTime += fDeltatime;
	}

}

void CEnemySigeTruck::Check_Delay(float fDeltatime)
{
	float fDelay = 0.15f;
	if (m_bIsFire && m_bIsInRange)
	{
		m_fElapsedTime += fDeltatime;
		if (m_fElapsedTime >= fDelay)
		{
			m_bIsFire = false;
			m_fElapsedTime = 0.f;
		}
	}
}

void CEnemySigeTruck::Chain_Explosion(float fDeltatime)
{
	float fDelay = 1.f;
	if (m_eCurEnemyState == DIE)
	{
		m_fElapsedTime += fDeltatime;
		if (!m_iExplosionCount)
		{
			++m_iExplosionCount;
			CObjManager::Get_Instance()->Add_Object(EXPLOSION, CAbstractFactory<CExplosion>::Create(m_tInfo.fX, m_tInfo.fY, -1, 600));
			CSoundMgr::Get_Instance()->PlaySound(L"Explosion_C4.wav", EXPLOSION_SOUND, 0.6f);
			CScreenManager::Instance().CamShake(9.f, 0.3f);
		}
		else if (m_fElapsedTime >= fDelay && m_iExplosionCount < 3)
		{
			CScreenManager::Instance().CamShake(9.f, 0.3f);
			CSoundMgr::Get_Instance()->PlaySound(L"Explosion_C4.wav", EXPLOSION_SOUND, 0.6f);
			CObjManager::Get_Instance()->Add_Object(EXPLOSION, CAbstractFactory<CExplosion>::Create(m_tInfo.fX + (-m_iExplosionCount * 50), m_tInfo.fY + (-m_iExplosionCount * 50), -1, 600));
			++m_iExplosionCount;
			m_fElapsedTime = 0.f;
		}
	}

}

