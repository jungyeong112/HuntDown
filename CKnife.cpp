#include "pch.h"
#include "CKnife.h"
#include "CAbstarctFactory.h"
#include "CThrow_Knife.h"
#include "CObjManager.h"


CKnife::CKnife()
{
}

CKnife::~CKnife()
{
}

void CKnife::Initialize()
{
	m_eType = KNIFE;
	m_fCoolTime = 3.f;
	m_fThrowSpeed = 600.f;
	m_iQuantitiy = 999;
}

void CKnife::Update()
{


}

void CKnife::Release()
{

}

void CKnife::Fire()
{
	for (int i = 0; i < 3; ++i)
	{
		//3발 발사, pos로 x 위치 차이 주기.
		CObjManager::Get_Instance()->Add_Object(BULLET, CAbstractFactory<CThrow_Knife>::Create((m_vFirePos.fx + i * 20.f), (m_vFirePos.fy - abs((i - 1) * 10)), m_iDir, m_fThrowSpeed));
	}
}
