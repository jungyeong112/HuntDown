#pragma once
#include "Define.h"

enum UI_ID
{
	HP_BAR, WEAPON_IMAGE, NUMBER, PLAYER_IMAGE , UI_END
};
class CUI
{
public:
	CUI();
	virtual ~CUI();
public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

public:

	//Get
	bool Get_Delete() { return m_bIsDelete; }
	bool Get_Active() { return m_bSetActive; }

	//Set
	void Set_Delete() { m_bIsDelete = true; }
	void Set_Activefalse() { m_bSetActive = false; }
	void Set_ActiveTrue() { m_bSetActive = true; }
	void Set_CameraPos(Vector2 vecPos) { m_vCameraPos = vecPos;}

protected:
	float m_fElapsedTime{ 0.f };          //중첩 시간 체크용
	bool m_bIsDelete{ false };            //UI 삭제 여부
	bool m_bSetActive{ false };           //ON_OFF 여부
	bool m_bIsFixed{ false };             //카메라에 고정 여부
	
	INFO m_tInfo;                         //출력위치용 중점,size
 	RECT m_tRect;                         //출력위치

	Vector2 m_vCameraPos;                  //카메라 위치
};

