#include"pch.h"
#include "CEnemy_AK47.h"
#include "CAbstarctFactory.h"
#include "CAk_Bullet.h"

CEnemy_AK47::CEnemy_AK47()
{
}

CEnemy_AK47::~CEnemy_AK47()
{
}

void CEnemy_AK47::Initialize()
{
	m_eGunType = AK_47;
	m_fFireDelay = 0.4;
	m_iDamage = 30.f;
	m_iMagazineCapacity = 50;
	m_IsAutoFire = true;
	m_fBulletSpeed = 600.f;
}

void CEnemy_AK47::Update()
{
}

void CEnemy_AK47::Release()
{
}

void CEnemy_AK47::Fire()
{
	if (m_iMagazineCapacity > 0)
		--m_iMagazineCapacity;
	wstring str = to_wstring(m_iMagazineCapacity) + L"- ³²À½ Uzi:\n";
	OutputDebugString(str.c_str());
	CObjManager::Get_Instance()->Add_Object(ENEMYBULLET, CAbstractFactory<CAk_Bullet>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir,m_fBulletSpeed));
}
