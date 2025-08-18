#pragma once

#include "Define.h"

class CObj;

enum ECollisionFlag
{
	CF_None,
	CF_Top,
	CF_Bottom,
	CF_Left,
	CF_Right,
	CF_ALL,
};

struct FCollision
{
	CObj* m_pObject;
	ECollisionFlag	m_Collisiontype;
	OBJID			m_OBJID;

	double			m_fX;
	double			m_fY;
};

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() PURE;
	virtual int  Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

public:
	//Get
	INFO Get_Info() { return m_tInfo; }
	bool Get_Dead() { return m_bIsDead; }
	bool Get_Hide() { return m_bIsHide; }
	CObj* Get_Target() { return m_pTarget; }
	int Get_Hp() { return m_iCurHp; }
	int Get_MaxHp() { return m_iMaxHp; }
	RECT Get_Rect() { return m_tRect; }
	Vector2 Get_Size();
	VECTOR2 Get_Pos();
	float Get_Angle() { return m_fAngle; }

	//Set
	void Set_Dead() { m_bIsDead = true; }
	void Set_Pos(float fx, float fy) { m_tInfo.fX = fx, m_tInfo.fY = fy; }
	void Set_Speed(float fSpeed) { m_fSpeed = fSpeed; }
	void Set_Size(float fCX, float fCY) { m_tInfo.fCX = fCX, m_tInfo.fCY = fCY; }
	void Set_Hide() { m_bIsHide = true; }
	void Set_Target(CObj* pobj) { m_pTarget = pobj; }
	void Set_Damage(int _iDamage) { m_iCurHp -= _iDamage; }
	void Set_FrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }
	void Set_LegFrame(int _start, int _end, int _motion, DWORD _speed, bool isLoop = true, DWORD _time = GetTickCount());
	void Set_BodyFrame(int _start, int _end, int _motion, DWORD _speed, bool isLoop = true, DWORD _time = GetTickCount());
	void Set_Dir(int _iDir) { m_iPlayerDir = _iDir; }
	void Add_CurHp(int ihp) { m_iCurHp += ihp; }
	void Set_Angle(float fAngle) { m_fAngle = fAngle; }
	virtual void OnCollision(FCollision _pCollision) {};


protected:
	void Update_Rect();
	void Move_LegFrame();
	void Move_BodyFrame();


protected:
	INFO m_tInfo;                //중점 fx,fy 와 Xsize, Ysize
	RECT m_tRect;                //실제 Rect

	FRAME m_tLegFrame;           //하체 애니메이션 프레임
	FRAME m_tBodyFrame;          //상체 애니메이션 프레임 

	float m_fSpeed{ 0.f };
	float m_fAngle{ 0.f };
	float m_fFireDelay{ 0.2f };   //사격 딜레이

	bool m_bIsDead{ false };      //사망 여부
	bool m_bIsHide{ false };      //엄폐 여부
	bool m_bIsHideArea{ false };  //숨을 수 있는 장소에 있는지
	bool m_bIsFire{ false };       //사격 여부
	bool m_bIsReLoad{ false };    //장전 중인지 
	bool m_bIsThrowAble{ true };  //보조무기 쿨타임 중인지
	bool m_bIsHit{ false };       //피격 여부

	bool m_bisLoopAnim{ true };    //상체 애니메이션 루프 여부
	bool m_bIsLegLoopAnim{ true }; //하체 애니메이션 루프 여부

	bool m_bisResetAnim{ false };  //애니메이션 리셋용   
	bool m_bBodyLock{ false };     //애니메이션 제어 상체
	bool m_isDash{ false };        //대시 여부
	bool m_bIsJump{ false };       //점프 중인지  
	bool m_bIsMaxJump{ false };    //점프하고 떨어지는 중인지

	int m_iCurHp{ 0 };             //현재 체력
	int m_iMaxHp{ 5 };             //최대 체력
	int m_iPlayerDir = +1;      // -1: 왼쪽, +1: 오른쪽 플레이어 보는 방향

	float m_dwLastFireTime{ 0 }; //마지막 사격시간

	const TCHAR* m_pFrameKey;    //애니메이션 Gun , 좌, 우 구분 
	const TCHAR* m_pLegFrameKey;   //다리 애니메이션 좌,우 구분용
	CObj* m_pTarget;             //타겟 지정용
	float m_fElapsedTime{ 0.f };        //중첩 시간
	float m_fThrowElapsedTime{ 0.f };   //보조무기 쿨타임 시간체크용
};

bool IsIntersect(const RECT& _rect1, const RECT& _rect2);


