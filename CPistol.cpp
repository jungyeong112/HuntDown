#include"pch.h"
#include "CPistol.h"
#include "CAbstarctFactory.h"
#include "CBullet.h"
#include "CSoundMgr.h"
#include "CEffectManager.h"
#include "CMuzzle_Flash.h"


CPistol::CPistol()
{
}

CPistol::~CPistol()
{
}

void CPistol::Initialize()
{
	m_eGunType = PISTOL;
	m_fFireDelay = 0.30f;
	m_iDamage = 20.f;
	m_iMagazineCapacity = 999;
	m_IsAutoFire = false;
	m_fBulletSpeed = 500.f;
	
}

int  CPistol::Update()
{
	return 1;
}

void CPistol::Release()
{
}

void CPistol::Fire()
{
	m_bSetActiveEffect = true;
	CEffectManager::Get_Instance()->Add_EFFECT(MUZZLE_FLASH, CAbstractFactory<CMuzzle_Flash>::CreateEffect(m_vFirePos.fx+20.f, m_vFirePos.fy, m_iDir,CObjManager::Get_Instance()->Get_Player()));
	CSoundMgr::Get_Instance()->PlaySound(L"MainMowMan.wav",SOUND_EFFECT,0.7f);
	CObjManager::Get_Instance()->Add_Object(BULLET, CAbstractFactory<CBullet>::Create(m_vFirePos.fx,m_vFirePos.fy,m_iDir,m_fBulletSpeed));
}
