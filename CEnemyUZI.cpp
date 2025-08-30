#include"pch.h"
#include "CEnemyUzi.h"
#include "CAbstarctFactory.h"
#include "CBullet.h"
#include "CSoundMgr.h"
#include "CEffectManager.h"
#include "CMuzzle_Flash.h"
#include "CMuzzle_Uzi.h"

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
	m_vFirePosOffset = { 0, 10 };
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
	CEffectManager::Get_Instance()->Add_EFFECT(MUZZLE_FLASH, CAbstractFactory<CMuzzle_Uzi>::CreateEffect(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_pOwner));
	CSoundMgr::Get_Instance()->PlaySound(L"Enemy_Uzi_Loop.wav", ENEMY_FIRE, 0.7f);
	CObjManager::Get_Instance()->Add_Object(ENEMYBULLET, CAbstractFactory<CBullet>::Create(m_vFirePos.fx, m_vFirePos.fy+10, m_iDir));
}
