#pragma once
#include "CObj.h"

enum ITEMTAG
{
	ITEM_POTION, ITEM_UZI, ITEM_SHOTGUN, ITEM_AK47, ITEM_GRENADE, ITEM_COLLECTION, ITEM_END
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
	int  Get_Magazine() { return m_iMagazine; }
	void Set_Magazine(int iMagazine) { m_iMagazine = iMagazine; }
	void Select_Anim();

protected:
	ITEMTAG m_eTag = ITEM_END;
	int m_iMagazine{ 30 };
};

