#include"pch.h"
#include "CEnemySigeTruck.h"
#include "CAbstarctFactory.h"
#include "CUIManager.h"
#include "CUI_EnemyHp.h"
#include "TimeManager.h"
#include  "CBmpMgr.h"
#include "CObjManager.h"
#include "CAk_Bullet.h"
#include "CExplosion.h"

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

	CUIManager::Get_Instance()->Add_UI(HP_BAR, CAbstractFactory<CUI_EnemyHp>::Create_UI(this));
	Set_LegFrame(0, 3, 0, 200.f);
}

int CEnemySigeTruck::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	Check_Delay(fDeltaTime);
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
		m_tRect.left, m_tRect.top - 200,
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
		}
		else
			m_eCurEnemyState = DIE;
	}
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
		CObjManager::Get_Instance()->Add_Object(ENEMYBULLET, CAbstractFactory<CAk_Bullet>::Create(m_tInfo.fX, m_tInfo.fY, -1, 600));
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
	float fDelay = 0.3f;
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
		}
		else if (m_fElapsedTime >= fDelay && m_iExplosionCount <3)
		{
			CObjManager::Get_Instance()->Add_Object(EXPLOSION, CAbstractFactory<CExplosion>::Create(m_tInfo.fX+(-m_iExplosionCount*50), m_tInfo.fY+(-m_iExplosionCount * 50), -1, 600));
			++m_iExplosionCount;
			m_fElapsedTime = 0.f;
		}
	}

}

