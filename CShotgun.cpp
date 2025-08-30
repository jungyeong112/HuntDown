#include "pch.h"
#include "CShotgun.h"
#include "CAbstarctFactory.h"
#include "CShot_Bullet.h"
#include "CShot_BulletEffect.h"
#include "CSoundMgr.h"
#include "CEffectManager.h"
#include "CScreenManager.h"
#include "CMuzzle_Shotgun.h"

CShotgun::CShotgun()
{
}

CShotgun::~CShotgun()
{
}

void CShotgun::Initialize()
{
	m_eGunType = SHOTGUN;
	m_fFireDelay = 1.3f;
	m_iDamage = 50;
	m_iMagazineCapacity = 30;
	m_IsAutoFire = false;
	m_fBulletSpeed = 600.f;
}

int  CShotgun::Update()
{
	return 0;
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


	CScreenManager::Instance().CamShake(5.f, 0.2f);
	CEffectManager::Get_Instance()->Add_EFFECT(MUZZLE_FLASH, CAbstractFactory<CMuzzle_Shotgun>::CreateEffect(m_vFirePos.fx + 20.f, m_vFirePos.fy, m_iDir, CObjManager::Get_Instance()->Get_Player()));
	CSoundMgr::Get_Instance()->PlaySound(L"Shotgun.wav", SOUND_EFFECT, 0.9f);
	CObjManager::Get_Instance()->Add_Object(EFFECT, CAbstractFactory<CShot_BulletEffect>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_fBulletSpeed));
	for (int i = -1; i < 2; ++i)
	{
		CObjManager::Get_Instance()->Add_Object(BULLET, CAbstractFactory<CShot_Bullet>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_fBulletSpeed, (i * 3.f)));
	}

}
