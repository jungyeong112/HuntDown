#include"pch.h"
#include "CUI.h"
#include "CObj.h"

CUI::CUI()
{
}

CUI::~CUI()
{
}

void CUI::Update_Rect()
{
	m_tRect.left = int(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = int(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = int(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = int(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}
