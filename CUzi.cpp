#include"pch.h"
#include "CUzi.h"
#include "CAbstarctFactory.h"
#include "CBullet.h"
#include "CSoundMgr.h"
#include "CEffectManager.h"
#include "CMuzzle_Uzi.h"

CUzi::CUzi()
{
}

CUzi::~CUzi()
{
}

void CUzi::Initialize()
{
	m_eGunType = UZI;
	m_fFireDelay = 0.12f;
	m_iDamage = 20.f;
	m_iMagazineCapacity = 50;
	m_IsAutoFire = true;
	m_fBulletSpeed = 800.f;
}

int CUzi::Update()
{
	return 0;
}

void CUzi::Release()
{
}

void CUzi::Fire()
{
	if (m_iMagazineCapacity > 0)
		--m_iMagazineCapacity;
	CEffectManager::Get_Instance()->Add_EFFECT(MUZZLE_FLASH, CAbstractFactory<CMuzzle_Uzi>::CreateEffect(m_vFirePos.fx, m_vFirePos.fy, m_iDir, CObjManager::Get_Instance()->Get_Player()));
	CSoundMgr::Get_Instance()->PlaySound(L"UziLoop.wav", SOUND_EFFECT,0.7f);
	wstring str = to_wstring(m_iMagazineCapacity) + L"- ³²À½ Uzi:\n";
	OutputDebugString(str.c_str());
	CObjManager::Get_Instance()->Add_Object(BULLET, CAbstractFactory<CBullet>::Create(m_vFirePos.fx-(10.f*m_iDir), m_vFirePos.fy+13.f, m_iDir));
}
