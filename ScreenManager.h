#pragma once

#include "pch.h"
#include "Define.h"

class CScreenManager
{
    int ScreenCX = WINCX;
    int ScreenCY = WINCY;
    int ScreenFPS = WINFPS;
    int MapSize = 16;

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

        // 셰이크 초기화
        m_shakeOffset = { 0,0 };
        m_trauma = 0.f;
        m_shakeMagPx = 12.f;     // 기본 최대 진폭(픽셀)
        m_shakeFreqHz = 18.f;     // 기본 주파수(Hz)
        m_shakeDecay = 2.0f;     // 초당 트라우마 감소량
        m_shakeT = 0.0;
        // 위상 랜덤(간단히 틱 카운터 사용)
        DWORD now = GetTickCount();
        m_phaseX = (now % 6283) / 1000.f;          // ~[0,6.283)
        m_phaseY = ((now / 2) % 6283) / 1000.f;
    };

    void Update(double dt)
    {
        UpdateShake(dt);
    };

    void Clear() {
        PatBlt(m_backHDC,
            m_posCamera.x,
            m_posCamera.y,
            ScreenCX, ScreenCY,
            WHITENESS);
    }

    // 셰이크 오프셋을 포함해서 그리기(경계 안전)
    void Present() {
        // 소스 시작점 = 논리 카메라 + 셰이크
        POINT src{ m_posCamera.x + m_shakeOffset.x,
                   m_posCamera.y + m_shakeOffset.y };

        // 맵 경계 클램핑 (읽기 영역이 백버퍼 범위를 넘지 않도록)
        const LONG minX = 0;
        const LONG minY = 0;
        const LONG maxX = std::max(minX, LONG(MapSize * ScreenCX) - ScreenCX);
        const LONG maxY = std::max(minY, LONG(MapSize * ScreenCY) - ScreenCY);

        src.x = std::clamp(src.x, minX, maxX);
        src.y = std::clamp(src.y, minY, maxY);

        BitBlt(m_frontHDC, 0, 0, ScreenCX, ScreenCY, m_backHDC, src.x, src.y, SRCCOPY);
    };

    static CScreenManager& Instance() {
        static CScreenManager instance;
        return instance;
    }

    HDC   GetHDC() const { return m_backHDC; }

    // 논리 카메라(플레이어 추적 등) 위치 설정
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

    // ===== 셰이크 API =====
    // 이벤트 때 호출: magPx(최대 진폭), addTrauma(0~1), freqHz(선택)
    void KickShake(float magPx, float addTrauma, float freqHz = 18.f)
    {
        m_shakeMagPx = std::max(m_shakeMagPx, magPx);
        m_shakeFreqHz = freqHz;
        m_trauma = std::clamp(m_trauma + addTrauma, 0.f, 1.f);
    }

private:
    CScreenManager() {
        m_frontHDC = m_backHDC = nullptr;
        m_hFrontBitmap = m_hBackBitmap = m_hOldFrontBitmap = m_hOldBackBitmap = nullptr;
        m_posCamera = { 0,0 };
        m_camRect = { 0,0,WINCX,WINCY };
        m_iStageSize = 0;

        m_shakeOffset = { 0,0 };
        m_trauma = 0.f;
        m_shakeMagPx = 12.f;
        m_shakeFreqHz = 18.f;
        m_shakeDecay = 2.0f;
        m_shakeT = 0.0;
        m_phaseX = 0.f;
        m_phaseY = 0.f;
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
    // 셰이크 내부 업데이트
    void UpdateShake(double dt)
    {
        // 트라우마 감소
        if (m_trauma > 0.f) {
            m_trauma = std::max(0.f, m_trauma - (float)(m_shakeDecay * dt));
        }

        // 진폭: trauma^2로 감쇠
        const float amp = m_shakeMagPx * (m_trauma * m_trauma);
        if (amp <= 0.01f) {
            m_shakeOffset = { 0,0 };
            return;
        }

        // 시간 진행
        m_shakeT += dt;

        // 파형 (서로 다른 위상/주파수로 분리)
        const float w = (float)(2.0 * 3.14159265358979323846 * m_shakeFreqHz);
        const float nx = sinf(w * (float)m_shakeT + m_phaseX);
        const float ny = cosf(1.7f * w * (float)m_shakeT + m_phaseY);

        // 정수 스냅(픽셀 그리드)
        m_shakeOffset.x = (int)lroundf(nx * amp);
        m_shakeOffset.y = (int)lroundf(ny * amp);
    }

private:
    POINT   m_posCamera;    // 논리 카메라(흔들림 제외)
    RECT    m_camRect;

    HDC     m_frontHDC;
    HDC     m_backHDC;

    HBITMAP m_hFrontBitmap;
    HBITMAP m_hBackBitmap;

    HBITMAP m_hOldFrontBitmap;
    HBITMAP m_hOldBackBitmap;

    int     m_iStageSize{ 0 };

    // ===== 셰이크 상태 =====
    POINT   m_shakeOffset;   // 현재 프레임 오프셋
    float   m_trauma;        // 0~1
    float   m_shakeMagPx;    // 최대 진폭(px)
    float   m_shakeFreqHz;   // Hz
    float   m_shakeDecay;    // 초당 감소
    double  m_shakeT;        // 누적 시간
    float   m_phaseX;        // 위상
    float   m_phaseY;        // 위상
};
