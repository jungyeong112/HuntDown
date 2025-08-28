#pragma once
#include "Cobj.h"
#include "CGun.h"
#include "CSubWeapon.h"

class CPlayer :public CObj
{
public:
	enum LEG_STATE { IDLE, WALK, DASH, JUMP, SIT_DOWN, STAND_UP, KICK, TAKE_COVER, DOWN, LEG_STATE_END };
	enum BODY_STATE { BODY_IDLE, BODY_WALK, BODY_FIRE, BODY_RELOAD, BODY_THROW, BODY_TAKECOVER, BODY_STATE_END };
public:
	CPlayer();
	~CPlayer();


public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(FCollision _Collison) override;

public:
	void KeyInput();
	void Set_CameraPos();
	void Motion_Chage();
	void BodyMotion_Change();
	void Anim_Reset();
	void Dash();
	void Check_Delay();
	void Check_Magazine();
	void Change_MainSlot();                 //대기 슬롯 - 활성 슬롯 교체
	void Change_SubSlot();
	void Select_BodyAnimSheet();
	void FireWeapon();
	void Throw_Weapon();
	void KnockBack(float fDeltaTime);
	void Check_Distance(); 
	void Create_Kick();
	void CheckWalkEffect(float fDeltaTime);
	void ReloadSound();

	template <typename T>
	void PickUp_Gun(int iMagazine) 
	{
		m_aMainWeaponSlot[m_iDeActiveSlot].reset();
		m_aMainWeaponSlot[m_iDeActiveSlot] = std::make_unique<T>();
		m_aMainWeaponSlot[m_iDeActiveSlot]->Initialize();
		m_aMainWeaponSlot[m_iDeActiveSlot]->Set_Magazine(iMagazine);
	} 
	template <typename T>
	void PickUp_SubWeapon(int iQauntity)
	{
		m_aSubWeaponSlot[m_iDeActiveSlot].reset();
		m_aSubWeaponSlot[m_iDeActiveSlot] = std::make_unique<T>();
		m_aSubWeaponSlot[m_iDeActiveSlot]->Initialize();
		m_aSubWeaponSlot[m_iDeActiveSlot]->Set_Quantity(iQauntity);
	}

public:
	//Get
	float Get_SubWeaponElapsedTime() { return m_fThrowElapsedTime; }
	CGun::GUNTYPE Get_PlayerGunType();
	CSubWeapon::THROWTYPE Get_ThrowType();
	int Get_Main_Magazine();
	int Get_Sub_Quantity();
private:
	HDC hMemDC;

	
	LEG_STATE m_eCurState;
	LEG_STATE m_ePreState;

	BODY_STATE m_eBodyCurState;
	BODY_STATE m_eBodyPreState;


	float m_fDashSpeed = 600.f;
	float m_fDashRemain = 0.f;
	float m_fKickRange = 40.f;
	

	unique_ptr<CGun> m_aMainWeaponSlot[2];      //주무기 보관 포인터 배열
	unique_ptr<CSubWeapon>m_aSubWeaponSlot[2];  //보조 무기 보관 포인터 배열
	int m_iMainActiveSlot{ 0 };                 //무기 슬롯 인덱스 0 권총 , 1 다른 무기
	int m_iSubActiveSlot{ 0 };                  //보조무기 슬롯인덱스 0 나이프 1 다른무기
	const int m_iDeActiveSlot{ 1 };             //기본 주,보조 무기가 아닌 슬롯들의 인덱스
	wstring m_wBodykey{L""};                    //플레이어 상체 애니메이션 시트 관리 /방향 + 이름 + 무기 Enum
	wstring m_wLegkey{ L"" };
	bool m_bIsKickAble{ false };

	float  m_fDashElapsedTime{ 0 };
	float  m_fDashBeforeY{ 0 };
	float  m_fWalkElapsedTime{ 0 };
	int    m_iOriginDir{ 0 };
	

};

