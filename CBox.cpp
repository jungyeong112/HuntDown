#include "pch.h"
#include "CBox.h"
#include "CBmpMgr.h"
#include "CAbstarctFactory.h"
#include "CObjManager.h"
#include "CExplosion.h"
#include "CSoundMgr.h"
CBox::CBox()
{
}

CBox::~CBox()
{
}

void CBox::Initialize()
{
	m_tInfo = { 0.f,0.f, 30.f,43.f };
	m_eBoxType = WOOD_BOX;
	m_ObjId = BOX;
	m_iMaxHp = 12;
	m_iCurHp = m_iMaxHp;
	Set_BodyFrame(0, 7, m_eBoxType, 200.f);
}

int CBox::Update()
{
	if (m_iCurHp <= 0)
	{
		if (m_eBoxType == GAS_BARREL)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Explosion_Mine.wav", EXPLOSION_SOUND, 0.7f);
			CObjManager::Get_Instance()->Add_Object(EXPLOSION, CAbstractFactory<CExplosion>::Create(m_tInfo.fX, m_tInfo.fY - 50.f, m_iPlayerDir));
		}
		m_bIsDead = true;
	}
	CObj::Update_Rect();

	if (m_bIsDead)
		return OBJ_DIE;
	else
		return OBJ_NO_EVENT;

}

void CBox::LateUpdate()
{

}

void CBox::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Box_Anim");

	GdiTransparentBlt(hDC,
		m_tRect.left - 15, m_tRect.top -5 ,
		65.f, 65.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		32 * m_tBodyFrame.iStart,           //원본 - 복사 시작위치x
		34 * m_tBodyFrame.iMotion,          //원본 - 복사 시작위치 y
		32, 34,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                                    //마젠타

	//BOXTYPE * Motion으로 시트에서 출력할거임.
	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

	}
}

void CBox::Release()
{
}

void CBox::OnCollision(FCollision _pCollision)
{
	if (_pCollision.m_OBJID == BULLET || _pCollision.m_OBJID == ENEMYBULLET)
	{
		m_iCurHp -= _pCollision.m_pObject->Get_Damage();
		Change_Anim();
	}
	if (_pCollision.m_OBJID == BOXBREAKER) 
	{
		m_iCurHp = 0;
	}
}

void CBox::Change_Anim()
{
	if (m_tBodyFrame.iEnd > m_tBodyFrame.iStart)
	{
		if (m_iCurHp % 2 != 0 && m_iCurHp > 1) //피가 홀 수 일때  
			++m_tBodyFrame.iStart;
	}

}

void CBox::Set_Box(BOXTYPE eType, int iMaxHp, int iEndFrame)
{
	m_eBoxType = eType;
	m_iMaxHp = iMaxHp;
	m_iCurHp = m_iMaxHp;
	Set_BodyFrame(0, iEndFrame, m_eBoxType, 0.f);
}
