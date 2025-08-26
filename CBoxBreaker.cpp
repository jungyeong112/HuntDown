#include "pch.h"
#include "CBoxBreaker.h"

CBoxBreaker::CBoxBreaker()
{
}

CBoxBreaker::~CBoxBreaker()
{
}

void CBoxBreaker::Initialize()
{
	m_tInfo = { 100.f, 100.f, 300.f, 100.f };
}

void CBoxBreaker::OnCollision(FCollision _pCollision)
{
	if (_pCollision.m_OBJID == BOX)
	{
		m_bIsDead = true;
	}
}
