#include"pch.h"
#include "CEnemyUzi.h"
#include "CAbstarctFactory.h"
#include "CBullet.h"

CEnemyUZI::CEnemyUZI()
{
}

CEnemyUZI::~CEnemyUZI()
{
}

void CEnemyUZI::Initialize()
{
	m_eGunType = UZI;
	m_fFireDelay = 0.3f;
	m_iDamage = 20.f;
	m_iMagazineCapacity = 50;
	m_IsAutoFire = true;
	m_fBulletSpeed = 80.f;
}

int  CEnemyUZI::Update()
{
	return 0;
}

void CEnemyUZI::Release()
{
}

void CEnemyUZI::Fire()
{
	CObjManager::Get_Instance()->Add_Object(ENEMYBULLET, CAbstractFactory<CBullet>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir));
}
