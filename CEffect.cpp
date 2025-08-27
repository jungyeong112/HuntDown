#include "pch.h"
#include "CEffect.h"

CEffect::CEffect()
{
}

CEffect::~CEffect()
{
}

void CEffect::UpdateEffectRect()
{
	m_tEffectRect.left = int(m_tEffectInfo.fX - (m_tEffectInfo.fCX * 0.5f));
	m_tEffectRect.top = int(m_tEffectInfo.fY - (m_tEffectInfo.fCY * 0.5f));
	m_tEffectRect.right = int(m_tEffectInfo.fX + (m_tEffectInfo.fCX * 0.5f));
	m_tEffectRect.bottom = int(m_tEffectInfo.fY + (m_tEffectInfo.fCY * 0.5f));
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
