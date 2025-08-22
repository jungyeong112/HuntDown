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

void CItem::Select_Anim()
{
	switch (m_eTag)
	{
	case ITEM_UZI:
		Set_BodyFrame(0, 3, 0, 200.f);
		break;

	case ITEM_SHOTGUN:
		Set_BodyFrame(0, 3, 1, 200.f);
		break;
	case ITEM_AK47:
		Set_BodyFrame(0, 3, 2, 200.f);
		break;
	}
}
