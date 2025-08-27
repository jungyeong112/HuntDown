#include "pch.h"
#include "CUI_EnemyHp.h"
#include "CObj.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CUI_EnemyHp::CUI_EnemyHp()
{
}

CUI_EnemyHp::~CUI_EnemyHp()
{
}

void CUI_EnemyHp::Initialize()
{
	m_tInfo = { 0.f,0.f, 20.f,20.f };
	m_tBgInfo = { 0.f,0.f, 20.f,20.f };
}

int CUI_EnemyHp::Update()
{
	VECTOR2 targetPos = m_pTarget->Get_Pos();
	m_tInfo.fX = targetPos.fx - 50.f;
	m_tInfo.fY = targetPos.fy - 65.f;
	m_tBgInfo.fX = targetPos.fx - 260.f;
	m_tBgInfo.fY = targetPos.fy - 60.f;
	CUI::Update_Rect();
	Update_BGRect();

	return 0;
}

void CUI_EnemyHp::LateUpdate()
{
	
}

void CUI_EnemyHp::Render(HDC hDC)
{
	m_bSetActive = m_pTarget->Get_UIActive();
	int iHp = m_pTarget->Get_Hp();
	if (!m_bSetActive||iHp<=0)
		return;
	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"EnemyHP_bg");

	GdiTransparentBlt(hDC,
		m_tBgRect.left + 210.f, m_tBgRect.top - 10,
		130, 30,                           //12는 피격 박스와 스프라이트 크기 보정
		hBackDC,
		0,
		0,
		30, 10,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));

	int iNum = m_pTarget->Get_Hp();
	for (int i = 0; i < iNum; ++i)
	{
		m_tInfo.fX += 8.f;
		Update_Rect();
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_HP");

		GdiTransparentBlt(hDC,
			m_tRect.left, m_tRect.top,
			63, 22,                           //12는 피격 박스와 스프라이트 크기 보정
			hMemDC,
			0,
			0,
			8, 8,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}

}


void CUI_EnemyHp::Release()
{
}

void CUI_EnemyHp::Update_BGRect()
{
	m_tBgRect.left = int(m_tBgInfo.fX - (m_tBgInfo.fCX * 0.5f));
	m_tBgRect.top = int(m_tBgInfo.fY - (m_tBgInfo.fCY * 0.5f));
	m_tBgRect.right = int(m_tBgInfo.fX + (m_tBgInfo.fCX * 0.5f));
	m_tBgRect.bottom = int(m_tBgInfo.fY + (m_tBgInfo.fCY * 0.5f));
}

