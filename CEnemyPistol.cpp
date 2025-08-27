#include "pch.h"
#include "CEnemyPistol.h"
#include "CAbstarctFactory.h"
#include "CBullet.h"

CEnemyPistol::CEnemyPistol()
{

}

CEnemyPistol::~CEnemyPistol()
{

}
void CEnemyPistol::Initialize()
{
	m_eGunType = PISTOL;
	m_fFireDelay = 1.f;
	m_iDamage = 20.f;
	m_iMagazineCapacity = 999;
	m_IsAutoFire = false;
	m_fBulletSpeed = 500.f;
}
int CEnemyPistol::Update()
{
	return 0;
}

void CEnemyPistol::Release()
{
}

void CEnemyPistol::Fire()
{
	CObjManager::Get_Instance()->Add_Object(ENEMYBULLET, CAbstractFactory<CBullet>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_fBulletSpeed));
}
