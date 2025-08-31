#include "pch.h"
#include "CBatEnemy.h"
#include "CBmpMgr.h"
#include "CObjManager.h"
#include "CUI.h"
#include "CAbstarctFactory.h"
#include "CUIManager.h"
#include "CUI_EnemyHp.h"

CBatEnemy::CBatEnemy()
{
}

CBatEnemy::~CBatEnemy()
{
}

void CBatEnemy::Initialize()
{
	m_tInfo = { 100.f,200.f , 40.f,70.f };
	m_iMaxHp = m_iCurHp = 7;
	m_fPatternTime = 1.f;
	m_fPlayerRange = 400.f;
	m_fSpeed = 200.f;
	m_eCurEnemyState = CHASE;
	m_bIsChase = true;
	m_ObjId = ENEMY;
	m_WeaponType = BAT;
	m_fKnockbackDistance = 150.f;
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

void CBatEnemy::Render(HDC hDC)
{
	m_pFrameKey = (m_iPlayerDir == +1) ? L"Bat_Enemy" : L"Bat_Enemy_L";

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left - 40, m_tRect.top - 20,
		140.f, 90.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		75 * m_tLegFrame.iStart,           //원본 - 복사 시작위치x
		50 * m_tLegFrame.iMotion,           //원본 - 복사 시작위치 y
		75, 50,                                      //복사할 가로 세로 사이즈
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
