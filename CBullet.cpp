#include"pch.h"
#include "CBullet.h"
#include "TimeManager.h"
#include "CBmpMgr.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
}

void CBullet::Initialize()
{
	m_tInfo = { 100.f, 100.f, 8.f, 5.f };
	m_fSpeed = 1000.f;
	Set_BodyFrame(0, 4, 0, 100.f);
	m_iDamage = 1;
}

int CBullet::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	m_tInfo.fX += (m_iPlayerDir * m_fSpeed) * fDeltaTime;

	CObj::Update_Rect();
	Check_Die(fDeltaTime);
	if (!m_bIsDead)
		return OBJ_NO_EVENT;
	else
		return OBJ_DIE;
}

void CBullet::LateUpdate()
{
	CObj::Move_BodyFrame();
}

void CBullet::Render(HDC hDC)
{
	m_pFrameKey = (m_iPlayerDir == +1) ? L"Pistol_Bullet" : L"Pistol_Bullet_L";
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		15, 7,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		8.f * m_tBodyFrame.iStart,
		8.f * m_tBodyFrame.iMotion,
		8.f, 5.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                  //마젠타

	if (DebugMode)
	{
		Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	}
}

void CBullet::Release()
{
}

void CBullet::OnCollision(FCollision _pCollision)
{
	if ((_pCollision.m_OBJID == ENEMY || _pCollision.m_OBJID == PLAYER) && _pCollision.m_pObject->Get_Hp() > 0 && !_pCollision.m_pObject->Get_Hide() || _pCollision.m_OBJID == WALL || _pCollision.m_OBJID == BOX 
		||_pCollision.m_OBJID==GROUND)
	{
		m_bIsDead = true;
	}
}

void CBullet::Check_Die(float fDeltaTime)
{
	m_fElapsedTime += fDeltaTime;
	if (m_fElapsedTime >= m_fDeathTime)
	{
		m_bIsDead = true;
	}
}
