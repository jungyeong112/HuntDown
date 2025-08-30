#include "pch.h"
#include "CPlayerBloodEffect.h"
#include "CBmpMgr.h"
#include "TimeManager.h"

CPlayerBloodEffect::CPlayerBloodEffect()
{
}

CPlayerBloodEffect::~CPlayerBloodEffect()
{
}

void CPlayerBloodEffect::Initialize()
{
    m_tInfo = { 0,0,40.f,40.f };
    Set_BodyFrame(0, 3, 0, 70.f, false);

}

int CPlayerBloodEffect::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	m_tInfo.fX = m_pTarget->Get_Pos().fx;
	m_tInfo.fY = m_pTarget->Get_Pos().fy;
	ActiveFalseTimer(fDeltaTime);
	Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;
}

void CPlayerBloodEffect::LateUpdate()
{
	Move_BodyFrame();
	if (m_tBodyFrame.iStart > m_tBodyFrame.iEnd)
	{
		m_bIsDead = true;
	}
}

void CPlayerBloodEffect::Render(HDC hDC)
{
	if (m_pTarget->Get_ID() == PLAYER) 
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PlayerBlood");

		GdiTransparentBlt(hDC,
			m_tRect.left - 15, m_tRect.top - 30,
			100, 100,
			hMemDC,
			100.f * m_tBodyFrame.iStart,
			0,
			100.f, 100.f,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 0));
	}
	else
	{
		
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"EnemyBlood");

		GdiTransparentBlt(hDC,
			m_tRect.left - 20, m_tRect.top - 30,
			100, 100,
			hMemDC,
			100.f * m_tBodyFrame.iStart,
			0,
			100.f, 100.f,                    //복사할 가로 세로 사이즈
			RGB(255, 0, 255));
	}
	
}

void CPlayerBloodEffect::Release()
{
}
