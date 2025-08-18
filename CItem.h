#pragma once
#include "CObj.h"
enum ITEMTAG
{
	ITEM_POTION, ITEM_UZI, ITEM_SHOTGUN, ITEM_AK47,ITEM_GRENADE, ITEM_END
};

class CItem :public CObj
{
public:
	CItem();
	virtual ~CItem();

public:
	void Set_Tag(ITEMTAG eTag) { m_eTag = eTag; }
	ITEMTAG Get_Tag() { return m_eTag; }
	void OnCollision(FCollision _pCollision);
	int  Get_Magazine() { return iMagazine; }

protected:
	ITEMTAG m_eTag = ITEM_END;
	int iMagazine{ 30 };
};

