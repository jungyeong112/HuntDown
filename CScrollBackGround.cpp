#include "pch.h"
#include "CScrollBackGround.h"

CScrollBackGround::CScrollBackGround()
{
}

CScrollBackGround::~CScrollBackGround()
{

}

void CScrollBackGround::Initialize(HDC backDC, int backW, int backH, float parallax, int yWorld)
{
	HDC m_backDC = backDC;
	int m_ibackW = backW;
	int m_ibackH = backH;
	float m_fparallax = parallax;
	int m_yWorld = yWorld;
}

void CScrollBackGround::Render(HDC hDC, int camX)
{
	int iOffset = (int)floorf(camX * m_fparallax);
	int res = SafeMod(iOffset, m_ibackW);
	int XWorld = camX - res;

	const int needWidth = WINCX + m_ibackW * 2;
	int drawCount = (needWidth / m_ibackW) + 2; // 최소 3장

	int xWorld = XWorld - m_ibackW;

	for (int i = 0; i < drawCount; ++i)
	{
		// 목적지(월드) 사각
		int dstX = xWorld;
		int dstY = m_yWorld;
		//TransparentBlt(hDC, dstX, dstY, m_ibackW, m_ibackH, m_backDC, 0, 0, m_ibackW, m_ibackH, RGB(255, 0, 255));
		xWorld += m_ibackW;
	}
}
