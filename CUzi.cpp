#include"pch.h"
#include "CUzi.h"
#include "CAbstarctFactory.h"
#include "CBullet.h"

CUzi::CUzi()
{
}

CUzi::~CUzi()
{
}

void CUzi::Initialize()
{
	m_eGunType = UZI;
	m_fFireDelay = 0.066f;
	m_iDamage = 20.f;
	m_iMagazineCapacity = 50;
	m_IsAutoFire = true;
	m_fBulletSpeed = 800.f;
}

void CUzi::Update()
{
}

void CUzi::Release()
{
}

void CUzi::Fire()
{
	if (m_iMagazineCapacity > 0)
		--m_iMagazineCapacity;
	wstring str = to_wstring(m_iMagazineCapacity) + L"- ³²À½ Uzi:\n";
	OutputDebugString(str.c_str());
	CObjManager::Get_Instance()->Add_Object(BULLET, CAbstractFactory<CBullet>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir));
}
