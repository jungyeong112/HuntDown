#pragma once
#include "pch.h"
#include "Define.h"

class CScreenManager
{
    int ScreenCX = WINCX;
    int ScreenCY = WINCY;
    int ScreenFPS = WINFPS;
    int MapSize = 16;

    // 카메라 흔들림
    float m_shakeTime = 0.f;
    float m_shakeDuration = 0.15f;
    float m_shakeAmp = 8.f;
    POINT m_shakeOffset = { 0,0 };

public:
    CScreenManager(const CScreenManager&) = delete;
    CScreenManager& operator=(const CScreenManager&) = delete;

    // === 새로 추가: 스무딩용 상태 ===
private:
    POINT m_posCamera;         // 실제(렌더에 쓰는) 카메라
    POINT m_targetCamera;      // 목표 카메라
    RECT  m_camRect;
    int   m_iStageSize{ 0 };

    // 스무딩 파라미터 (원하면 조절)
    float m_smoothTime = 0.20f;   // 목표까지 도달 “특성 시간”(초). 작을수록 빠르게 붙음
    float m_velX = 0.f;           // 1D 스프링 속도(내부 상태)
    float m_velY = 0.f;

    // 1D 크리티컬 감쇠 스무스댐프(유니티 SmoothDamp 유사)
    static float SmoothDamp1D(float current, float target, float& currentVelocity, float smoothTime, float dt)
    {
        // 안전장치
        if (smoothTime < 0.0001f) smoothTime = 0.0001f;

        float omega = 2.0f / smoothTime;
        float x = current - target;
        float exp = expf(-omega * dt);
        float temp = (currentVelocity + omega * x) * dt;
        float newVal = target + (x + temp) * exp;
        currentVelocity = (currentVelocity - omega * temp) * exp;
        return newVal;
    }

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

        // 초기값
        m_posCamera = { 0,0 };
        m_targetCamera = { 0,0 };
        m_camRect = { 0,0,WINCX,WINCY };
        m_velX = m_velY = 0.f;
    }

    void Clear() {
        PatBlt(m_backHDC,
            m_posCamera.x,
            m_posCamera.y,
            ScreenCX, ScreenCY,
            WHITENESS);
    }

    void Update(double dt)
    {
        // === 흔들림 갱신 (기존) ===
        if (m_shakeTime > 0.f) {
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

        // === 스무딩: m_posCamera -> m_targetCamera 로 점진 이동 ===
        // (X, Y 각각 스무스댐프)
        float cx = (float)m_posCamera.x;
        float cy = (float)m_posCamera.y;
        float tx = (float)m_targetCamera.x;
        float ty = (float)m_targetCamera.y;

        cx = SmoothDamp1D(cx, tx, m_velX, m_smoothTime, (float)dt);
        cy = SmoothDamp1D(cy, ty, m_velY, m_smoothTime, (float)dt);

        // 정수화 + 클램핑
        m_posCamera.x = std::clamp((int)lroundf(cx), 0, std::max(0, m_iStageSize - WINCX));
        m_posCamera.y = std::clamp((int)lroundf(cy), 0, MapSize * WINCY);

        SetCameraRect();
    }

    void Present() {
        POINT src{ m_posCamera.x + m_shakeOffset.x,
                   m_posCamera.y + m_shakeOffset.y };

        src.x = std::max(0L, std::min(src.x, (LONG)(MapSize * ScreenCX - ScreenCX)));
        src.y = std::max(0L, std::min(src.y, (LONG)(MapSize * ScreenCY - ScreenCY)));

        BitBlt(m_frontHDC, 0, 0, ScreenCX, ScreenCY, m_backHDC, src.x, src.y, SRCCOPY);
    }

    static CScreenManager& Instance() {
        static CScreenManager instance;
        return instance;
    }

    HDC   GetHDC() const { return m_backHDC; }

    // === 기존 SetCameraPos 는 “즉시 스냅” 용도로 유지 (필요시)
    void SetCameraPos(POINT p, bool clampMap = true)
    {
        if (clampMap) {
            p.x = std::clamp((int)p.x, 0, std::max(0, m_iStageSize - WINCX));
            p.y = std::clamp((int)p.y, 0, MapSize * WINCY);
        }
        m_posCamera = p;
        m_targetCamera = p;     // 스냅이므로 목표도 동일
        m_velX = m_velY = 0.f;  // 속도 리셋
        SetCameraRect();
    }

    // === 새로 추가: “목표”만 설정 → Update에서 부드럽게 따라감
    void SetCameraTarget(POINT p, bool clampMap = true)
    {
        if (clampMap) {
            p.x = std::clamp((int)p.x, 0, std::max(0, m_iStageSize - WINCX));
            p.y = std::clamp((int)p.y, 0, MapSize * WINCY);
        }
        m_targetCamera = p;
        // 속도는 유지(자연스러운 이동)
    }

    // 스무딩 강도 조절(원하면 노출)
    void SetSmoothTime(float smoothSec) { m_smoothTime = std::max(0.01f, smoothSec); }

    // 스테이지 크기/카메라 사각
    void  Set_StageSize(int iDst) { m_iStageSize = iDst; }
    POINT GetCamerPos() const { return m_posCamera; }         // (흔들림 제외)
    RECT  GetCamRect() const { return m_camRect; }

    void  SetCameraRect() {
        m_camRect = { m_posCamera.x, m_posCamera.y,
                      m_posCamera.x + ScreenCX, m_posCamera.y + ScreenCY };
    }

    // 흔들림 트리거
    void CamShake(float amp = 8.f, float duration = 0.15f)
    {
        m_shakeAmp = amp;
        m_shakeDuration = duration;
        m_shakeTime = duration;
    }

private:
    CScreenManager() {
        m_frontHDC = m_backHDC = nullptr;
        m_hFrontBitmap = m_hBackBitmap = m_hOldFrontBitmap = m_hOldBackBitmap = nullptr;
        m_posCamera = { 0,0 };
        m_targetCamera = { 0,0 };
        m_camRect = { 0,0,WINCX,WINCY };
        m_iStageSize = 0;
    }
    ~CScreenManager() { Release(); }

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
    }

private:
    HDC     m_frontHDC = nullptr;
    HDC     m_backHDC = nullptr;

    HBITMAP m_hFrontBitmap = nullptr;
    HBITMAP m_hBackBitmap = nullptr;

    HBITMAP m_hOldFrontBitmap = nullptr;
    HBITMAP m_hOldBackBitmap = nullptr;
};
