#pragma once
#include "CalculateUtil.h"
class CSubWeapon
{
public:
	enum THROWTYPE
	{
		KNIFE,
		GRENADE,
		THORW_END
	};

public:
	CSubWeapon();
	virtual ~CSubWeapon();

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Release() PURE;
	virtual void Fire() PURE;

public:
	THROWTYPE Get_Type() { return m_eType; }
	float  Get_CoolTime() { return m_fCoolTime;}
	void  Set_FirePos(Vector2 vFirePos, int iDir) { m_vFirePos = CalculateUtil::Add(vFirePos, m_vFirePosOffset), m_iDir = iDir; }

protected:
	float m_fCoolTime{ 0.0f };
	float m_fThrowSpeed{ 0.0f };
	int m_iDamae{ 0 };
	THROWTYPE m_eType{ KNIFE };
	Vector2 m_vFirePos{ 0,0 };
	Vector2 m_vFirePosOffset{ 0,0 };
	int m_iDir{ 1 };
	float m_fElapsedTime{ 0.f };
	bool m_bIsThrowAble{ true};
};

