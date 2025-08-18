#pragma once

#include "CObj.h"
#include "CBox.h"
#include "Weapon_Item.h"

template<typename T>
class CAbstractFactory
{
public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create(float fX, float fY, int iDir,  float fSpeed = 0.f,float fAngle = 0.f)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(fX, fY);
		pObj->Set_Dir(iDir);
		if (fSpeed)
			pObj->Set_Speed(fSpeed);
		if (fAngle)
			pObj->Set_Angle(fAngle);
		return pObj;
	}
	static CObj* CreateBox(float fX, float fY, BOXTYPE eType, int MaxHp, int EndFrame)
	{
		CBox* pObj = new CBox;
		pObj->Initialize();
		pObj->Set_Pos(fX, fY);
		pObj->Set_Box(eType, MaxHp, EndFrame);
		return pObj;
	}
	static CObj* CreateItem(float fX, float fY, ITEMTAG eType)
	{
		CObj* pObj = new Weapon_Item;
		pObj->Initialize();
		pObj->Set_Pos(fX, fY);
		//pObj->Set_Tag(eType);
		return pObj;
	}


};

