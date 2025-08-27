#pragma once

#include "CObj.h"
#include "CBox.h"
#include "Weapon_Item.h"
#include "CUI.h"
#include "CShootingEnemy.h"
#include "CSubWeapon_Item.h"
#include "CEffect.h"

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

	static CObj* Create(float fX, float fY, int iDir, float fSpeed = 0.f, float fAngle = 0.f)
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
		pItem->Set_Magazine(iMagazine);
		pItem->Set_Pos(fX, fY);
		pItem->Initialize();
		pItem->Set_Tag(eType);
	
		pItem->Select_Anim();
		return pItem;
	}
	static CItem* CreateSubItem(float fx, float fy, ITEMTAG eType, int iQuantity)
	{
		CItem* pItem = new CSubWeapon_Item;
		pItem->Set_Magazine(iQuantity);
		pItem->Set_Pos(fx, fy);
		pItem->Initialize();
		pItem->Set_Tag(eType);
		return pItem;
	}
	static CUI* Create_UI()
	{
		CUI* pUI = new T;
		pUI->Initialize();

		return pUI;
	}
	static CUI* Create_UI(CObj* pObj, int iQuantity = 0)
	{
		CUI* pUI = new T;
		pUI->Set_Target(pObj);
		pUI->Initialize();
		if (iQuantity)
			pUI->Set_Quantity(iQuantity);

		return pUI;
	}
	static CEffect* CreateEffect(float fX, float fY, int iDir, CObj* pTarget)
	{
		CEffect* pEffect = new T;
		pEffect->Initialize();
		pEffect->Set_Pos(fX, fY);
		pEffect->Set_Dir(iDir);
		pEffect->Set_Target(pTarget);


		return pEffect;
	}

};

