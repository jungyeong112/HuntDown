#include "pch.h"
#include "CPlayer.h"
#include "CBmpMgr.h"
#include "TimeManager.h"
#include "CKeyMgr.h"
#include "CalculateUtil.h"
#include "ScreenManager.h"
#include "CPistol.h"
#include "CUzi.h"
#include "CAK_47.h"
#include "CItem.h"
#include "Weapon_Item.h"
#include "CGun.h"
#include "CShotgun.h"
#include "CKnife.h"
#include "CGrenade.h"
#include "CMelee.h"
#include "CAbstarctFactory.h"

CPlayer::CPlayer()
{

}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo = { 100.f, 200.f, 40.f, 70.f };//x ,y 좌표와 크기
	m_ObjId = PLAYER;

	m_iMaxHp = 5;
	m_iCurHp = 4;

	m_fSpeed = 200.f;
	m_eCurState = IDLE;
	m_eBodyCurState = BODY_IDLE;

	OriginCY = m_tInfo.fCY;
	SitCY = OriginCY - 20.f;

	m_vCurVelocity = { 0,0 };
	m_vCurDirection = { 0,0 };
	m_vCurAccerelation = { 0, DGRAVITY };

	m_aMainWeaponSlot[m_iMainActiveSlot] = make_unique<CPistol>();
	m_aMainWeaponSlot[m_iMainActiveSlot]->Initialize();

	//m_aMainWeaponSlot[1] = make_unique<CUzi>();
	//m_aMainWeaponSlot[1]->Initialize();

	m_aSubWeaponSlot[m_iSubActiveSlot] = make_unique<CGrenade>();
	m_aSubWeaponSlot[m_iSubActiveSlot]->Initialize();



	CObj::Update_Rect();
}

int CPlayer::Update()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	ApplyGravity(fDeltaTime);
	Check_Delay();
	Check_Magazine();
	Check_Distance();
	if (m_eCurState != DOWN)
		KeyInput();
	KnockBack(fDeltaTime);
	Dash();
	Anim_Reset();
	Set_CameraPos();

	CObj::Update_Rect();

	return OBJ_NO_EVENT;
}


void CPlayer::LateUpdate()
{
	Motion_Chage();
	BodyMotion_Change();

	CObj::Move_BodyFrame();
	CObj::Move_LegFrame();
	Throw_Weapon();
}



void CPlayer::Render(HDC hDC)
{
	int iOffSet = (m_iPlayerDir == +1) ? 17.f : 5.f;
	m_pFrameKey = m_wBodykey.c_str();
	m_pLegFrameKey = m_wLegkey.c_str();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pLegFrameKey);       //다리

	GdiTransparentBlt(hDC,
		m_tRect.left, m_tRect.top,
		90,90,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		48.f * m_tLegFrame.iStart,
		48.f * m_tLegFrame.iMotion,         //복사 시작 위치
		48.f, 48.f,                         //복사할 가로 세로 사이즈
		RGB(179, 121, 59));                  //삭제 할 색상


	HDC hBodyMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);  //상체
	if (m_eCurState != DOWN)
	{
		GdiTransparentBlt(hDC,
			m_tRect.left+(iOffSet), m_tRect.top - 30,
			70.f, 70.f,
			hBodyMemDC,
			37.f * m_tBodyFrame.iStart,
			32.f * m_tBodyFrame.iMotion,            //복사 시작 위치
			37.f, 32.f,
			RGB(255, 0, 255));
	}

	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

	}

}


void CPlayer::Release()
{

}

void CPlayer::OnCollision(FCollision _Collison)
{
	if (_Collison.m_OBJID == GROUND || _Collison.m_OBJID == BOX)
	{
		if (_Collison.m_Collisiontype == CF_Bottom)
		{
			Set_CollisionPos(_Collison.m_fY);
		}
		else if (_Collison.m_Collisiontype == CF_Right)
		{
			m_tInfo.fX -= _Collison.m_fX;
		}
		else if (_Collison.m_Collisiontype == CF_Left)
		{
			m_tInfo.fX += _Collison.m_fX;
		}
	}

	if (_Collison.m_OBJID == FLAT_GROUND)
	{
		if (_Collison.m_Collisiontype == CF_Bottom && !m_bIsMaxJump && m_eCurState != DOWN)
		{
			Set_CollisionPos(_Collison.m_fY);
			m_bIsDownJumpable = true;
		}
	}

	if (_Collison.m_OBJID == WALL)
	{
		if (_Collison.m_Collisiontype == CF_Right)
		{
			m_tInfo.fX -= m_tInfo.fCX * 0.5f;
		}
		else if (_Collison.m_Collisiontype == CF_Left)
		{
			m_tInfo.fX += m_tInfo.fCX * 0.5f;
		}
	}
	if (_Collison.m_OBJID == ENEMY_MELEE)
	{
		m_eCurState = DOWN;
	}

	if (_Collison.m_OBJID == ITEM)
	{
		CItem* pObj = static_cast<CItem*>(_Collison.m_pObject);
		ITEMTAG eTag = pObj->Get_Tag();
		bool PickUp = CKeyMgr::Get_Instance()->Key_Pressing('X');
		switch (eTag)
		{
		case ITEM_UZI:
			if (!m_aMainWeaponSlot[m_iDeActiveSlot] || m_aMainWeaponSlot[m_iDeActiveSlot]->Get_Type() != CGun::GUNTYPE::UZI)
			{
				if (PickUp)
				{
					PickUp_Gun<CUzi>();
					_Collison.m_pObject->Set_Dead();
					Change_MainSlot();
				}
			}
			else
			{
				OutputDebugString(L"UZI 중복");
				m_aMainWeaponSlot[m_iDeActiveSlot]->Add_Magazine(pObj->Get_Magazine());
				_Collison.m_pObject->Set_Dead();
			}

			break;
		case ITEM_AK47:
			if (!m_aMainWeaponSlot[m_iDeActiveSlot] || m_aMainWeaponSlot[m_iDeActiveSlot]->Get_Type() != CGun::GUNTYPE::AK_47)
			{
				if (PickUp)
				{
					PickUp_Gun<CAK_47>();
					_Collison.m_pObject->Set_Dead();
					Change_MainSlot();
				}
			}
			else
			{
				OutputDebugString(L"Ak 중복");
				m_aMainWeaponSlot[m_iDeActiveSlot]->Add_Magazine(pObj->Get_Magazine());
				_Collison.m_pObject->Set_Dead();
			}
			break;

		case ITEM_SHOTGUN:
			if (!m_aMainWeaponSlot[m_iDeActiveSlot] || m_aMainWeaponSlot[m_iDeActiveSlot]->Get_Type() != CGun::GUNTYPE::SHOTGUN)
			{
				if (PickUp)
				{
					PickUp_Gun<CShotgun>();
					_Collison.m_pObject->Set_Dead();
					Change_MainSlot();
				}
			}
			else
			{
				OutputDebugString(L"Ak 중복");
				m_aMainWeaponSlot[m_iDeActiveSlot]->Add_Magazine(pObj->Get_Magazine());
				_Collison.m_pObject->Set_Dead();
			}
			break;
		}
	}
	if (_Collison.m_OBJID == HIDE_AREA)
	{
		m_bIsHideArea = true;
	}
	else if (_Collison.m_OBJID != HIDE_AREA)
	{
		m_bIsHideArea = false;
	}
	Set_Pos(m_tInfo.fX, m_tInfo.fY);
	CObj::Update_Rect();
}

void CPlayer::KeyInput()
{
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	bool bAutoFire = m_aMainWeaponSlot[m_iMainActiveSlot]->Get_AutoFire();
	if (CKeyMgr::Get_Instance()->Key_Down('C') && m_eCurState != TAKE_COVER)
	{
		OutputDebugString(L"단발\n");
		FireWeapon();
		m_bBodyLock = true;

	}
	if (CKeyMgr::Get_Instance()->Key_Pressing_Only('C') && m_eCurState != TAKE_COVER)
	{
		if (bAutoFire)
		{
			OutputDebugString(L"연사\n");
			FireWeapon();
		}
	}

	if (GetAsyncKeyState(VK_RIGHT) && !m_bIsHide && m_eCurState != SIT_DOWN && m_eCurState != TAKE_COVER)
	{
		m_tInfo.fX += m_fSpeed * fDeltaTime;
		m_pFrameKey = L"Player_Right";
		m_eCurState = WALK;

		if (!m_bBodyLock)
			m_eBodyCurState = BODY_WALK;
		m_iPlayerDir = +1;
	}
	if (GetAsyncKeyState(VK_LEFT) && !m_bIsHide && m_eCurState != SIT_DOWN && m_eCurState != TAKE_COVER)
	{
		m_tInfo.fX -= m_fSpeed * fDeltaTime;
		m_pFrameKey = L"Player_Left";
		m_eCurState = WALK;

		if (!m_bBodyLock)
			m_eBodyCurState = BODY_WALK;
		m_iPlayerDir = -1;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('Z') && m_bIsThrowAble)
	{
		m_eBodyCurState = BODY_THROW;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('V') && !m_bIsJump && m_bJumpable)
	{
		if (m_eCurState != SIT_DOWN)
		{
			m_eCurState = JUMP;
		}
		if (m_eCurState == SIT_DOWN && m_bIsDownJumpable)
		{
			m_tInfo.fY += 40.f;
			m_eCurState = JUMP;
		}
	}
	else if (GetAsyncKeyState(VK_UP) && !m_bBodyLock && m_bIsHideArea && !m_bIsJump && !m_isDash && !m_bIsHit)
	{
		m_eCurState = TAKE_COVER;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		m_eCurState = SIT_DOWN;
	}
	else if (CKeyMgr::Get_Instance()->Key_Up(VK_DOWN))
	{
		m_eCurState = IDLE;
	}

	else if (CKeyMgr::Get_Instance()->Key_Down('B') && !m_isDash && m_eCurState != SIT_DOWN && m_eCurState != TAKE_COVER)
	{
		m_eCurState = DASH;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('X') && m_aMainWeaponSlot[m_iDeActiveSlot])
	{
		Change_MainSlot();
	}
	else if (CKeyMgr::Get_Instance()->Key_Up(VK_UP) || CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT) || CKeyMgr::Get_Instance()->Key_Up(VK_LEFT)
		|| CKeyMgr::Get_Instance()->Key_Up(VK_DOWN) || CKeyMgr::Get_Instance()->Key_Up('V'))
	{
		if (!CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
			m_eCurState = IDLE;
		m_bIsHit = false;
	}
	else    //키 떼면

	{
		m_bIsHide = false;
		if (m_eCurState != SIT_DOWN)m_tInfo.fCY = OriginCY;
		if (!m_isDash)
		{
			m_fSpeed = 200.f;
		}
	}
	Select_BodyAnimSheet();
}



void CPlayer::Set_CameraPos()
{
	POINT _pt = { -(WINCX >> 1), -(WINCY) };
	_pt.x += (int)m_tInfo.fX;
	_pt.y += (int)m_tInfo.fY;

	CScreenManager::Instance().SetCameraPos(_pt);
}

void CPlayer::Motion_Chage()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			if (!m_bBodyLock)
				CObj::Set_BodyFrame(0, 0, 0, 200);
			CObj::Set_LegFrame(0, 5, 0, 200);
			break;

		case CPlayer::WALK:
			if (m_isDash)
				CObj::Set_LegFrame(0, 7, 2, 150, false);
			else
				CObj::Set_LegFrame(0, 3, 1, 200);
			break;

		case CPlayer::DASH:
			CObj::Set_LegFrame(0, 7, 2, 130, false);
			m_isDash = true;
			m_fDashRemain = 0.3f;

			break;

		case CPlayer::JUMP:
			m_bIsJump = true;
			m_bIsMaxJump = true;
			if (m_eCurState != SIT_DOWN)
				CObj::Set_LegFrame(0, 5, 3, 200, false);
			m_vCurVelocity.fy = -DJUMPSPEED;
			break;

		case CPlayer::SIT_DOWN:
			CObj::Set_LegFrame(0, 1, 4, 200, false);
			m_tInfo.fCY = SitCY;
			break;

		case CPlayer::STAND_UP:
			CObj::Set_LegFrame(0, 2, 5, 200, false);
			break;

		case CPlayer::KICK:
			Create_Kick();
			CObj::Set_LegFrame(0, 3, 6, 200.f, false);
			m_bIsKickAble = false;
			break;

		case CPlayer::TAKE_COVER:
			m_bIsHide = true;
			CObj::Set_BodyFrame(0, 1, 5, 1000);
			CObj::Set_LegFrame(0, 2, 7, 200);
			break;

		case CPlayer::DOWN:
			m_bIsHit = true;
			m_bIsJump = true;
			m_bIsMaxJump = true;
			CObj::Set_LegFrame(0, 4, 8, 200, false);
			m_vCurVelocity.fy = -DJUMPSPEED;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::BodyMotion_Change()
{

	if (m_eBodyCurState != m_eBodyPreState)
	{
		m_bisResetAnim = false;
		switch (m_eBodyCurState)
		{
		case CPlayer::BODY_IDLE:
			CObj::Set_BodyFrame(0, 0, 0, 200);
			break;
		case CPlayer::BODY_WALK:
			CObj::Set_BodyFrame(0, 1, 1, 200);
			break;
		case CPlayer::BODY_FIRE:
			CObj::Set_BodyFrame(0, 1, 2, 100, false);
			m_bBodyLock = true;
			break;
		case CPlayer::BODY_RELOAD:
			CObj::Set_BodyFrame(0, 1, 3, 200, false);
			m_bBodyLock = true;
			break;
		case CPlayer::BODY_THROW:
			CObj::Set_BodyFrame(0, 1, 4, 200, false);
			m_bBodyLock = true;
			break;
		}
		m_eBodyPreState = m_eBodyCurState;
	}
}

void CPlayer::Anim_Reset()
{
	if (m_bisResetAnim)
	{
		m_eBodyCurState = BODY_IDLE;
		m_bBodyLock = false;
	}
}

void CPlayer::Dash()
{
	if (!m_isDash) return;
	m_tInfo.fCY = SitCY;
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

	m_tInfo.fX += (m_iPlayerDir * m_fDashSpeed) * fDeltaTime;
	m_fDashRemain -= fDeltaTime;// 지속시간 소모
	if (m_fDashRemain <= 0.f)
	{
		m_isDash = false;
		m_eCurState = IDLE;
	}
}

void CPlayer::Check_Delay()
{
	float fFireDelay = m_aMainWeaponSlot[m_iMainActiveSlot]->Get_FireDelay();
	float fDeltaTime = TimeManager::GetInstance()->GetDeltaTime();

	m_fElapsedTime += fDeltaTime;
	if (m_fElapsedTime >= fFireDelay)
	{
		m_bIsFire = false;
		m_fElapsedTime -= fFireDelay;
	}

	float fThrowDelay = m_aSubWeaponSlot[m_iSubActiveSlot]->Get_CoolTime();
	m_fThrowElapsedTime += fDeltaTime;
	if (m_fThrowElapsedTime >= fThrowDelay)
	{
		m_bIsThrowAble = true;
		m_fThrowElapsedTime -= fThrowDelay;
	}
}

void CPlayer::Check_Magazine()
{
	int iMagazine = m_aMainWeaponSlot[m_iMainActiveSlot]->Get_MagazineCapacity();
	if (iMagazine <= 0)
	{
		Change_MainSlot();
		m_aMainWeaponSlot[1].reset();
		m_aMainWeaponSlot[1] = nullptr;
		OutputDebugString(L"총알 0\n");

	}
}

void CPlayer::Change_MainSlot()
{
	if (m_aMainWeaponSlot[1] == NULL)
		return;
	else
		m_iMainActiveSlot = !m_iMainActiveSlot ? 1 : 0;
	//m_bBodyLock = true;
	m_eBodyCurState = BODY_RELOAD;
}

void CPlayer::Change_SubSlot()
{
	if (m_aSubWeaponSlot[m_iDeActiveSlot] == NULL)
		return;
	else
		m_iSubActiveSlot = !m_iSubActiveSlot ? 1 : 0;
}

void CPlayer::Select_BodyAnimSheet()
{
	int GunType = m_aMainWeaponSlot[m_iMainActiveSlot]->Get_Type();
	m_wBodykey = to_wstring(m_iPlayerDir) + L"PlayerBody" + to_wstring(GunType);
	m_wLegkey = to_wstring(m_iPlayerDir) + L"PlayerLeg";
}

void CPlayer::FireWeapon()
{
	if (m_bIsKickAble)
	{
		m_eCurState = KICK;
	}
	else if (!m_bIsFire && m_eCurState != KICK)
	{
		m_eBodyCurState = BODY_FIRE;
		m_aMainWeaponSlot[m_iMainActiveSlot]->Set_FirePos(Get_FirePos(), m_iPlayerDir);
		m_aMainWeaponSlot[m_iMainActiveSlot]->Fire();
		if (m_aMainWeaponSlot[m_iMainActiveSlot]->Get_Type() == CGun::GUNTYPE::SHOTGUN)
		{
			m_eBodyCurState = BODY_RELOAD;
		}
		m_bIsFire = true;
	}
}

void CPlayer::Throw_Weapon()
{
	if (m_bisResetAnim && m_eBodyCurState == BODY_THROW)
	{
		m_aSubWeaponSlot[m_iSubActiveSlot]->Set_FirePos(Get_FirePos(), m_iPlayerDir);
		m_aSubWeaponSlot[m_iSubActiveSlot]->Fire();
		m_bIsThrowAble = false;
		m_bIsDownJumpable = false;
	}
}

void CPlayer::KnockBack(float fDeltaTime)
{
	if (m_eCurState == DOWN && m_fKnockBackElapsedTime < m_fKnockBackTime)
	{
		m_fKnockBackElapsedTime += fDeltaTime;
		m_tInfo.fX += (-m_iPlayerDir * m_fKnockbackDistance) * fDeltaTime;
	}
	else if (m_eCurState == DOWN && m_fKnockBackElapsedTime >= m_fKnockBackTime)
	{
		m_eCurState = IDLE;
		m_fKnockBackElapsedTime -= m_fKnockBackTime;
	}

}

void CPlayer::Check_Distance()
{
	if (CObjManager::Get_Instance()->Get_Obj_InRange(ENEMY, m_tInfo.fX, m_tInfo.fY, m_fKickRange, m_iPlayerDir))
	{
		m_bIsKickAble = true;
	}
}

void CPlayer::Create_Kick()
{
	CObjManager::Get_Instance()->Add_Object(OBJID::KICK, CAbstractFactory<CMelee>::Create(Get_FirePos().fx , Get_FirePos().fy + 30.f, m_iPlayerDir));
}




