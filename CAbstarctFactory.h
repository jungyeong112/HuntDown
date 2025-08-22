#pragma once

#include "CObj.h"
#include "CBox.h"
#include "Weapon_Item.h"
#include "CUI.h"
#include "CShootingEnemy.h"
#include "CSubWeapon_Item.h"

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
	static CItem* CreateMainItem(float fX, float fY, ITEMTAG eType, int iMagazine)
	{
		CItem* pItem = new Weapon_Item;
		pItem->Initialize();
		pItem->Set_Pos(fX, fY);
		pItem->Set_Tag(eType);
		pItem->Set_Magazine(iMagazine);
		pItem->Select_Anim();
		return pItem;
	}
	static CItem* CreateSubItem(float fx, float fy, ITEMTAG eType, int iQuantity) 
	{
		CItem* pItem = new CSubWeapon_Item;
		pItem->Initialize();
		pItem->Set_Pos(fx, fy);
		pItem->Set_Tag(eType);
		pItem->Set_Magazine(iQuantity);
		return pItem;
	}
	static CUI* Create_UI()
	{
		CUI* pUI = new T;
		pUI->Initialize();

		return pUI;
	}
	static CUI* Create_UI(CObj* pObj)
	{
		CUI* pUI = new T;
		pUI->Initialize();
		pUI->Set_Target(pObj);

		return pUI;
	}



};

