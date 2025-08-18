#include "pch.h"
#include "CShotgun.h"
#include "CAbstarctFactory.h"
#include "CShot_Bullet.h"
#include "CShot_BulletEffect.h"


CShotgun::CShotgun()
{
}

CShotgun::~CShotgun()
{
}

void CShotgun::Initialize()
{
	m_eGunType = SHOTGUN;
	m_fFireDelay = 1.3;
	m_iDamage = 50;
	m_iMagazineCapacity = 15;
	m_IsAutoFire = false;
	m_fBulletSpeed = 600.f;
}

void CShotgun::Update()
{
}

void CShotgun::Release()
{
}

void CShotgun::Fire()
{
	if (m_iMagazineCapacity > 0)
		--m_iMagazineCapacity;
	wstring str = to_wstring(m_iMagazineCapacity) + L"- ³²À½,Shot:\n";
	OutputDebugString(str.c_str());

	CObjManager::Get_Instance()->Add_Object(EFFECT, CAbstractFactory<CShot_BulletEffect>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_fBulletSpeed));
	for (int i = -1; i < 2; ++i)
	{
		CObjManager::Get_Instance()->Add_Object(BULLET, CAbstractFactory<CShot_Bullet>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_fBulletSpeed, (i * 3.f)));
	}

}
