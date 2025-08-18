#include "pch.h"
#include "CItem.h"


CItem::CItem()
{
}

CItem::~CItem()
{
}

void CItem::OnCollision(FCollision _pCollision)
{
	if (_pCollision.m_OBJID == PLAYER) 
	{
		//m_bIsDead = true;
	}
}
