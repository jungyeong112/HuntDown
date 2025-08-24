#include "pch.h"
#include "CScrollBackGround.h"

CScrollBackGround::CScrollBackGround() {}
CScrollBackGround::~CScrollBackGround() {}

void CScrollBackGround::Initialize(HDC tileDC, int tileW, int tileH, float parallax, int yWorld)
{
	m_tileDC = tileDC;
	m_tileW = tileW;
	m_tileH = tileH;
	m_parallax = parallax;
	m_yWorld = yWorld;
}

void CScrollBackGround::Render(HDC hDC, int camX)
{

	if (!hDC || !m_tileDC) return;
	if (m_tileW <= 0 || m_tileH <= 0) return;


	const int iOffset = (int)floorf(camX * m_parallax);

	const int res = SafeMod(iOffset, m_tileW);

	const int XWorld = camX - res;


	const int needWidth = WINCX + m_tileW * 1;
	const int drawCount = (needWidth / m_tileW) + 2;

	int xWorld = XWorld - m_tileW;

	for (int i = 0; i < drawCount; ++i)
	{
		const int dstX = xWorld;
		const int dstY = m_yWorld;

		BOOL ok = TransparentBlt(
			hDC, dstX, dstY, m_tileW, m_tileH,   // 목적지(Rect)
			m_tileDC, 0, 0, m_tileW, m_tileH,    // 소스(전체)
			RGB(255, 0, 255)
		);
		int gap = 120;
		xWorld += (m_tileW + gap);
	}
}
