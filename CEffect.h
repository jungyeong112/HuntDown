#pragma once

#include "Define.h"
#include "CObj.h"
#include "CGun.h"

enum  EFFECTID
{
	MUZZLE_FLASH, BLOOD, DUST, MONSTER_EFFECT, EFFECT_END
};
class CEffect :public CObj
{
public:
	CEffect();
	virtual ~CEffect();

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

public:

	void UpdateEffectRect();

	//Get
	bool Get_Delete() { return m_bIsDelete; }
	bool Get_Active() { return m_bSetActive; }

	//Set
	void Set_Delete() { m_bIsDelete = true; }
	void Set_Activefalse() { m_bSetActive = false; }
	void Set_ActiveTrue() { m_bSetActive = true; }
	void Set_GunTarget(CGun* pGun) { m_pGunTarget = pGun; }

	void ActiveFalseTimer(float fDeltaTime);

protected:
	bool m_bSetActive{ true };
	bool m_bIsDelete{ false };

	float m_fActiveTime{ 1.f };
	float m_fElapsedTime{ 0.f };
	CGun* m_pGunTarget;

	INFO m_tEffectInfo;
	RECT m_tEffectRect;
};

