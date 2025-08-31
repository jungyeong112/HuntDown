#include "pch.h"
#include "CDoor.h"
#include "CSoundMgr.h"
#include "CBmpMgr.h"
#include "CAbstarctFactory.h"
#include "CFlagManager.h"

CDoor::CDoor()
{
}

CDoor::~CDoor()
{
}

void CDoor::Initialize()
{
	m_tInfo = { 0.f,0.f, 30.f,50.f };
	m_iMaxHp = m_iCurHp = 10;
	Set_BodyFrame(0, 7, 0, 200.f,false);
}

int CDoor::Update()
{
	if(m_bIsHit)
	{
		//m_bIsDead = true;
		
		CFlagManager::Get_Instance()->Set_Doorbreaching();
	}
	Update_Rect();
	return (m_bIsDead == true) ? OBJ_DIE : OBJ_NO_EVENT;
}

void CDoor::LateUpdate()
{
	if(m_bIsHit)
	Move_BodyFrame();
}

void CDoor::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Door");

	GdiTransparentBlt(hDC,
		m_tRect.left - 20 , m_tRect.top-7 ,
		120.f, 60.f,                           //12는 피격 박스와 스프라이트 크기 보정
		hMemDC,
		100 * m_tBodyFrame.iStart,           //원본 - 복사 시작위치x
		34 * m_tBodyFrame.iMotion,          //원본 - 복사 시작위치 y
		100, 50,                                      //복사할 가로 세로 사이즈
		RGB(255, 0, 255));                                    //마젠타


	if (DebugMode)
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

	}
}

void CDoor::Release()
{
}

void CDoor::OnCollision(FCollision _pCollision)
{
	if (_pCollision.m_OBJID == KICK) 
	{
		m_iCurHp = 0;
		CSoundMgr::Get_Instance()->PlaySoundW(L"GlassDoorSmash.wav", SOUND_EFFECT, 0.9f);
		m_bIsHit = true;
	}
	if (_pCollision.m_OBJID == BULLET) 
	{
		--m_iCurHp;

		if (2> m_tBodyFrame.iStart)
		{
			if (m_iCurHp % 2 != 0 && m_iCurHp > 1) 
				++m_tBodyFrame.iStart;
		}
	}
}
