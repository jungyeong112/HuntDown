#include "pch.h"
#include "CEffect.h"

CEffect::CEffect()
{
}

CEffect::~CEffect()
{
}

void CEffect::ActiveFalseTimer(float fDeltaTime)
{
	if (m_bSetActive)
	{
		m_fElapsedTime += fDeltaTime;
		if (m_fElapsedTime >= m_fActiveTime)
		{
			m_bIsDead = true;
			m_fElapsedTime -= m_fActiveTime;
		}
	}
}
