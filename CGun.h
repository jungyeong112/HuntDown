#pragma once

#include "CObjManager.h"
#include "CalculateUtil.h"
#include "CObj.h"

class CGun 
{
public:
	enum  GUNTYPE
	{
		PISTOL,
		UZI,
		SHOTGUN,
		AK_47,
		GUN_END,
	};

public:
	CGun();
	virtual ~CGun();

public:

	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Release() PURE;
	virtual void Fire() PURE;

public:

	float Get_FireDelay() { return m_fFireDelay;}   //발사 딜레이 get 
	void  Set_Owner(CObj* pObj) { m_pOwner = pObj; } //대상 설정
	void  Set_FirePos(Vector2 vFirePos, int iDir) { m_vFirePos = CalculateUtil::Add(vFirePos,m_vFirePosOffset), m_iDir = iDir; }
	void  Add_Magazine(int ibullet) { m_iMagazineCapacity += ibullet; }
	void  Set_Magazine(int ibullet) { m_iMagazineCapacity = ibullet; }
	bool  Get_EffectActive() { return m_bSetActiveEffect; }


	Vector2 Get_FirePos() { return m_vFirePos; }
	GUNTYPE Get_Type() { return m_eGunType; }
	int Get_MagazineCapacity() { return m_iMagazineCapacity;}
	bool Get_AutoFire() { return m_IsAutoFire; }
	
protected:
	float m_fFireDelay{ 0.0f }; 
	float m_fBulletSpeed{ 0.0f };
	int m_iDamage{ 0 };
	GUNTYPE m_eGunType{ PISTOL };
	int m_iMagazineCapacity{ 0 };
	Vector2 m_vFirePos{0,0};
	CObj* m_pOwner;  
	int m_iDir{ 1 }; //발사 방향 1 오른쪽 , -1 왼쪽
	bool m_IsAutoFire{ false };
	bool m_bSetActiveEffect{ false };
	Vector2 m_vFirePosOffset{ 0,0 };
};


