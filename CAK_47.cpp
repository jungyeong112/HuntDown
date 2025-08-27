#include "pch.h"
#include "CAK_47.h"
#include "CAbstarctFactory.h"
#include "CAk_Bullet.h"

CAK_47::CAK_47()
{
}

CAK_47::~CAK_47()
{
}

void CAK_47::Initialize()
{
	m_eGunType = AK_47;
	m_fFireDelay = 0.13f;
	m_iDamage = 30.f;
	m_iMagazineCapacity = 50;
	m_IsAutoFire = true;
	m_fBulletSpeed = 700.f;
}

int  CAK_47::Update()
{
	return 0;
}

void CAK_47::Release()
{
}

void CAK_47::Fire()
{
	if (m_iMagazineCapacity > 0)
		--m_iMagazineCapacity;
	wstring str = to_wstring(m_iMagazineCapacity) + L"- ³²À½ AK:\n";
	OutputDebugString(str.c_str());

    CObjManager::Get_Instance()->Add_Object(BULLET, CAbstractFactory<CAk_Bullet>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_fBulletSpeed));
}
