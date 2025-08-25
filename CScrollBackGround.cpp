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

	const int period = m_tileW + m_iGap;                     // 반복 주기
	const int iOffset = (int)(camX * m_parallax);
	const int res = SafeMod(iOffset, period);
	const int iBackMove = -camX - res;

	const int needWidth = WINCX + period * 2;

	const int drawCount = (needWidth / period) + 4;
	int backX = iBackMove * 0.05f;

	for (int i = 0; i < drawCount; ++i) {
		TransparentBlt(hDC, backX, m_yWorld, m_tileW, m_tileH,
			m_tileDC, 0, 0, m_tileW, m_tileH, RGB(255, 0, 255));
		backX += period;
	}
}
