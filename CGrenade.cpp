#include "pch.h"
#include "CGrenade.h"
#include "CAbstarctFactory.h"
#include "CObjManager.h"
#include "CThrow_Grenade.h"

CGrenade::CGrenade()
{
}

CGrenade::~CGrenade()
{
}

void CGrenade::Initialize()
{
	m_eType = GRENADE;
	m_fCoolTime = 0.5f;
	m_fThrowSpeed = 650.f;     //수류탄 높이 관여 (m_fSpeed)
}

void CGrenade::Update()
{
}

void CGrenade::Release()
{
}

void CGrenade::Fire()
{
	CObjManager::Get_Instance()->Add_Object(EFFECT, CAbstractFactory<CThrow_Grenade>::Create(m_vFirePos.fx, m_vFirePos.fy, m_iDir, m_fThrowSpeed));
}
