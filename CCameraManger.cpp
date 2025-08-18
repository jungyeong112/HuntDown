#include "pch.h"
#include "CCameraManger.h"
#include "CObj.h"

CCameraManger* CCameraManger::m_pInstance = nullptr;

CCameraManger::CCameraManger() :m_pTarget(nullptr)
{
}

CCameraManger::~CCameraManger()
{
}

VECTOR2 CCameraManger::Get_RenderPos(Vector2 _VobjPos)
{
	Vector2 vRes = CalculateUtil::Sub(_VobjPos, m_vDiffrence);
	return vRes;
}

void CCameraManger::Update()
{
	if (!m_pTarget)
		return;
	else
		m_vLookAt = { m_pTarget->Get_Info().fX ,m_pTarget->Get_Info().fY };
	Offset();
}

void CCameraManger::Offset()
{
	Vector2 vRes = { WINCX, WINCY };
	Vector2 vCenter = CalculateUtil::Div(vRes, 2.f);

	m_vDiffrence = CalculateUtil::Sub(m_vLookAt, vCenter);

}
