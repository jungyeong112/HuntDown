#pragma once

#include "pch.h"
#include "Define.h"

class CScreenManager
{
	int ScreenCX = WINCX;
	int ScreenCY = WINCY;
	int ScreenFPS = WINFPS;
	int MapSize = 16;
	float m_shakeTime = 0.f;   // 남은 시간
	float m_shakeDuration = 0.15f; // 총 지속
	float m_shakeAmp = 8.f;   // 최대 진폭(px)
	POINT m_shakeOffset = { 0,0 };
public:
	CScreenManager(const CScreenManager&) = delete;
	CScreenManager& operator=(const CScreenManager&) = delete;

public:
	void Initialize()
	{
		if (!m_frontHDC) {
			m_frontHDC = GetDC(g_hWnd);
		}
		if (!m_backHDC) {
			m_backHDC = CreateCompatibleDC(m_frontHDC);
			m_hBackBitmap = CreateCompatibleBitmap(m_frontHDC, MapSize * ScreenCX, MapSize * ScreenCY);
			m_hOldBackBitmap = (HBITMAP)SelectObject(m_backHDC, m_hBackBitmap);
		}

	};

	void Clear() {
		PatBlt(m_backHDC,
			m_posCamera.x,
			m_posCamera.y,
			ScreenCX, ScreenCY,
			WHITENESS);
	}

	void Update(double dt)
	{
		if (m_shakeTime > 0.f)
		{
			m_shakeTime -= (float)dt;
			if (m_shakeTime < 0.f) m_shakeTime = 0.f;

			float k = (m_shakeTime / m_shakeDuration); 
			int ox = (rand() % 3 - 1) * (int)(m_shakeAmp * k);     
			int oy = (rand() % 3 - 1) * (int)(m_shakeAmp * k * 0.6f);
			m_shakeOffset = { ox, oy };
		}
		else {
			m_shakeOffset = { 0,0 };
		}
	}
	void Present() {
		POINT src{ m_posCamera.x + m_shakeOffset.x,
				   m_posCamera.y + m_shakeOffset.y };

		src.x = std::max(0L, std::min(src.x, (LONG)(MapSize * ScreenCX - ScreenCX)));
		src.y = std::max(0L, std::min(src.y, (LONG)(MapSize * ScreenCY - ScreenCY)));

		BitBlt(m_frontHDC, 0, 0, ScreenCX, ScreenCY, m_backHDC, src.x, src.y, SRCCOPY);
	};

	static CScreenManager& Instance() {
		static CScreenManager instance;
		return instance;
	}

	HDC   GetHDC() const { return m_backHDC; }

	void SetCameraPos(POINT _cameraPos, bool _bClampingMap = true) {
		m_posCamera = _cameraPos;
		if (_bClampingMap) {
			m_posCamera.x = std::clamp((int)m_posCamera.x, 0, m_iStageSize - WINCX);
			m_posCamera.y = std::clamp((int)m_posCamera.y, 0, MapSize * WINCY);
		}
		SetCameraRect();
	}

	POINT GetCamerPos() const { return m_posCamera; }     // 논리 카메라(흔들림 제외)
	RECT  GetCamRect() const { return m_camRect; }
	void  Set_StageSize(int iDst) { m_iStageSize = iDst; }
	void  SetCameraRect() {
		m_camRect = { m_posCamera.x, m_posCamera.y,
					 m_posCamera.x + ScreenCX, m_posCamera.y + ScreenCY };
	}

private:
	CScreenManager() {
		m_frontHDC = m_backHDC = nullptr;
		m_hFrontBitmap = m_hBackBitmap = m_hOldFrontBitmap = m_hOldBackBitmap = nullptr;
		m_posCamera = { 0,0 };
		m_camRect = { 0,0,WINCX,WINCY };
		m_iStageSize = 0;
	};
	~CScreenManager() { Release(); };

	void Release() {
		if (m_frontHDC) {
			SelectObject(m_frontHDC, m_hOldFrontBitmap);
			DeleteObject(m_hFrontBitmap);
			ReleaseDC(g_hWnd, m_frontHDC);
			m_frontHDC = nullptr;
		}
		if (m_backHDC) {
			SelectObject(m_backHDC, m_hOldBackBitmap);
			DeleteObject(m_hBackBitmap);
			DeleteDC(m_backHDC);
			m_backHDC = nullptr;
		}
	};
public:

	void CamShake(float amp = 8.f, float duration = 0.15f)
	{
		m_shakeAmp = amp;
		m_shakeDuration = duration;
		m_shakeTime = duration;
	}
	

private:
	POINT   m_posCamera;   
	RECT    m_camRect;

	HDC     m_frontHDC;
	HDC     m_backHDC;

	HBITMAP m_hFrontBitmap;
	HBITMAP m_hBackBitmap;

	HBITMAP m_hOldFrontBitmap;
	HBITMAP m_hOldBackBitmap;

	int     m_iStageSize{ 0 };
};
