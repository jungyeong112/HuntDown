#include "pch.h"
#include "CObj.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "TimeManager.h"



CObj::CObj() : m_fSpeed(0.f)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CObj::~CObj()
{
}

Vector2 CObj::Get_Size()
{
	Vector2 vSize = { m_tInfo.fCX,m_tInfo.fCY };
	return vSize;
}

VECTOR2 CObj::Get_Pos()
{
	Vector2 vPos = { m_tInfo.fX,m_tInfo.fY };
	return vPos;
}

void CObj::Set_LegFrame(int _start, int _end, int _motion, DWORD _speed, bool isLoop, DWORD _time)
{
	m_tLegFrame.iStart = _start;
	m_tLegFrame.iEnd = _end;
	m_tLegFrame.iMotion = _motion;
	m_tLegFrame.dwSpeed = _speed;
	m_tLegFrame.dwTime = _time;
	m_bIsLegLoopAnim = isLoop;
}
void CObj::Set_BodyFrame(int _start, int _end, int _motion, DWORD _speed, bool isLoop, DWORD _time)
{
	m_tBodyFrame.iStart = _start;
	m_tBodyFrame.iEnd = _end;
	m_tBodyFrame.iMotion = _motion;
	m_tBodyFrame.dwSpeed = _speed;
	m_tBodyFrame.dwTime = _time;
	m_bisLoopAnim = isLoop;
}


void CObj::Update_Rect()
{
	m_tRect.left = int(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = int(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = int(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = int(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Move_LegFrame()
{
	if (m_tLegFrame.dwTime + m_tLegFrame.dwSpeed < GetTickCount())
	{
		++m_tLegFrame.iStart;
		m_tLegFrame.dwTime = GetTickCount();

		if (m_tLegFrame.iStart > m_tLegFrame.iEnd)
		{
			if (m_bIsLegLoopAnim)
				m_tLegFrame.iStart = 0;
			else
			{
				m_tLegFrame.iStart = m_tLegFrame.iEnd;
			}
			m_bIsAnimEnd = true;
		}
		else m_bIsAnimEnd = false;
	}
}
void CObj::Move_BodyFrame()
{
	if (m_tBodyFrame.dwTime + m_tBodyFrame.dwSpeed < GetTickCount())
	{
		++m_tBodyFrame.iStart;

		m_tBodyFrame.dwTime = GetTickCount();
		if (m_tBodyFrame.iStart > m_tBodyFrame.iEnd)
		{
			if (m_bisLoopAnim)
				m_tBodyFrame.iStart = 0;
			else
			{
				m_bisResetAnim = true;
			}
		}
	}

}
void CObj::UI_ActiveTimer(float fDeltaTime)
{
	if (m_bUISetActive)
	{
		m_fUIElapsedTime += fDeltaTime;
		if (m_fUIElapsedTime >= m_fUIActiveTime)
		{
			m_bUISetActive = false;
			m_fUIElapsedTime -= m_fUIActiveTime;
			m_bInvencible = false;
		}
	}
}

Vector2 CObj::Get_FirePos()
{
	VECTOR2 vFirePos = Get_Pos();
	vFirePos.fx += (m_iPlayerDir * 30.f);

	if (m_tInfo.fCY == SitCY)
		vFirePos.fy -= 20.f;
	else
		vFirePos.fy -= 30.f;

	return vFirePos;
}
void CObj::ApplyGravity(float fDeltaTime)
{
	//float fdeltaTime = TimeManager::GetInstance()->GetDeltaTime();
	const float kMaxDt = 1.0f / 30.0f;                     //제한
	if (fDeltaTime > kMaxDt) fDeltaTime = kMaxDt;

	m_vCurVelocity.fy += m_vCurAccerelation.fy * fDeltaTime;


	m_tInfo.fX += m_vCurVelocity.fx * fDeltaTime;
	m_tInfo.fY += m_vCurVelocity.fy * fDeltaTime;

	if (m_bIsJump && m_tInfo.fY <= HighY || m_tInfo.fY >= LowY)   //점프 최고점 체크
	{
		m_bIsMaxJump = false;
	}
	if (m_vCurVelocity.fy > 47.f)   //떨어지는 중 점프 막기
	{
		m_bJumpable = false;
	}
	Set_Pos(m_tInfo.fX, m_tInfo.fY);
}

void CObj::Set_CollisionPos(float _fy)
{
	m_bIsJump = false;
	m_vCurVelocity.fy = 0.0;
	m_vCurVelocity.fx = 0.0;
	OriginY = m_tInfo.fY;
	HighY = OriginY - 94.f;
	LowY = OriginY + 50.f;
	m_bJumpable = true;
	m_bIsDownJumpable = false;
	m_tInfo.fY -= _fy;
}

bool IsIntersect(const RECT& _rect1, const RECT& _rect2)
{
	if ((_rect1.bottom - _rect1.top) + (_rect2.bottom - _rect2.top) < (max(_rect1.bottom, _rect2.bottom) - min(_rect1.top, _rect2.top))) return false;
	if ((_rect1.right - _rect1.left) + (_rect2.right - _rect2.left) < (max(_rect1.right, _rect2.right) - min(_rect1.left, _rect2.left))) return false;
	return true;
}

