#include "pch.h"
#include "CAk_Bullet.h"
#include "TimeManager.h"
#include "CBmpMgr.h"

CAk_Bullet::CAk_Bullet()
{
}

CAk_Bullet::~CAk_Bullet()
{
}

void CAk_Bullet::Initialize()
{
	m_tInfo = { 100.f, 100.f, 15.f, 6.f };
	m_fSpeed = 1000.f;
	Set_BodyFrame(0, 1, 0, 100.f);
}

int CAk_Bullet::Update()
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

void CAk_Bullet::LateUpdate()
{
	Move_BodyFrame();
}

void CAk_Bullet::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"AK_Bullet");

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		20, 8,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		14.f * m_tBodyFrame.iStart,
		6.f * m_tBodyFrame.iMotion,
		14.f, 6.f,                    //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                  //마젠타

	if (DebugMode)
	{
		Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	}
}

void CAk_Bullet::Release()
{
}

void CAk_Bullet::OnCollision(FCollision _pCollision)
{
	if (_pCollision.m_OBJID == ENEMY && _pCollision.m_pObject->Get_Hp() > 0 && !_pCollision.m_pObject->Get_Hide())
	{
		m_bIsDead = true;
	}
	if ( _pCollision.m_OBJID == WALL || _pCollision.m_OBJID == BOX)
	{
		m_bIsDead = true;
	}
	
}

void CAk_Bullet::Check_Die(float fdeltaTime)
{
	m_fElapsedTime += fdeltaTime;
	if (m_fElapsedTime >= m_fDeathTime)
	{
		m_bIsDead = true;
	}
}
