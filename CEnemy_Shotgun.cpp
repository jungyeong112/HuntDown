#include "pch.h"
#include "CEnemy_Shotgun.h"
#include "CAbstarctFactory.h"
#include "CShot_BulletEffect.h"
#include "CShot_Bullet.h"

CEnemy_Shotgun::CEnemy_Shotgun()
{
}

CEnemy_Shotgun::~CEnemy_Shotgun()
{
}

void CEnemy_Shotgun::Initialize()
{

	m_eGunType = SHOTGUN;
	m_fFireDelay = 1.3f;
	m_iDamage = 50;
	m_iMagazineCapacity = 30;
	m_IsAutoFire = false;
	m_fBulletSpeed = 600.f;
}

int CEnemy_Shotgun::Update()
{
	return 0;
}

void CEnemy_Shotgun::Release()
{
}

void CEnemy_Shotgun::Fire()
{
	if (m_iMagazineCapacity > 0)
		--m_iMagazineCapacity;
	wstring str = to_wstring(m_iMagazineCapacity) + L"- ³²À½,Shot:\n";
	OutputDebugString(str.c_str());

	CObjManager::Get_Instance()->Add_Object(EFFECT, CAbstractFactory<CShot_BulletEffect>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_fBulletSpeed));
	for (int i = -1; i < 2; ++i)
	{
		CObjManager::Get_Instance()->Add_Object(ENEMYBULLET, CAbstractFactory<CShot_Bullet>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_fBulletSpeed, (i * 3.f)));
	}
}
