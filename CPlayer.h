#pragma once
#include "Cobj.h"
#include "CGun.h"
#include "CSubWeapon.h"

class CPlayer :public CObj
{
public:
	enum LEG_STATE { IDLE, WALK, DASH, JUMP, SIT_DOWN, STAND_UP, KICK, TAKE_COVER, DOWN, FIRE, LEG_STATE_END };
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
	void ApplyGravity();
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
	Vector2 Get_FirePos();
	void Set_CollisionPos(float _fy);

	template <typename T>
	void PickUp_Gun() 
	{
		m_aMainWeaponSlot[m_iDeActiveSlot].reset();
		m_aMainWeaponSlot[m_iDeActiveSlot] = std::make_unique<T>();
		m_aMainWeaponSlot[m_iDeActiveSlot]->Initialize();
	} 

private:
	HDC hMemDC;


	LEG_STATE m_eCurState;
	LEG_STATE m_ePreState;

	BODY_STATE m_eBodyCurState;
	BODY_STATE m_eBodyPreState;

	bool m_bIsSit{ false };
	bool m_bJumpable{ true };
	bool m_bIsPressing{ false };
	bool m_bIsDownJumpable{ false };

	float m_fDashSpeed = 600.f;
	float m_fDashRemain = 0.f;

	float OriginCY{ 0.f };
	float SitCY{ 0.f };
	
	float OriginY{ 0.f };
	float HighY{ 0.f };
	float LowY{ 0.f };
	
	constexpr static double DMAXSPEED = 500.0f;
	constexpr static double DJUMPSPEED = 770.f;
	constexpr static double DGRAVITY = 3000.0f;

	VECTOR2 m_vCurVelocity;               //현재 속도
	VECTOR2 m_vCurDirection;              //현재 방향
	Vector2 m_vCurAccerelation;           //가속도

	unique_ptr<CGun> m_aMainWeaponSlot[2];      //주무기 보관 포인터 배열
	unique_ptr<CSubWeapon>m_aSubWeaponSlot[2];  //보조 무기 보관 포인터 배열
	int m_iMainActiveSlot{ 0 };                      //무기 슬롯 인덱스 0 권총 , 1 다른 무기
	int m_iSubActiveSlot{ 0 };                    //보조무기 슬롯인덱스 0 나이프 1 다른무기
	const int m_iDeActiveSlot{ 1 };                      //기본 주,보조 무기가 아닌 슬롯들의 인덱스
	wstring m_wBodykey{L""};                    //플레이어 상체 애니메이션 시트 관리 /방향 + 이름 + 무기 Enum
	wstring m_wLegkey{ L"" };
};

