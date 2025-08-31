#pragma once
#include "pch.h"
#include "Define.h"
#include <algorithm>
#include <cmath>

#pragma comment(lib, "msimg32.lib") // AlphaBlend용

class CScreenManager
{
    // 화면/맵 기본
    int ScreenCX = WINCX;
    int ScreenCY = WINCY;
    int ScreenFPS = 60;
    int MapSize = 16;

    // --- 카메라(스무딩 추적) ---
    POINT m_posCamera{ 0,0 };    // 실제 카메라(렌더 소스)
    POINT m_targetCamera{ 0,0 };    // 목표 카메라(플레이어가 던짐)
    RECT  m_camRect{ 0,0, WINCX, WINCY };
    float m_smoothTime = 0.20f;    // 작을수록 빠르게 목표로 수렴
    float m_velX = 0.f;      // 감쇠 내부 속도 상태(X)
    float m_velY = 0.f;      // 감쇠 내부 속도 상태(Y)

    // --- 스테이지/경계 ---
    int   m_iStageSize = 0;        // 맵 전체 폭(월드)
    int   m_scrollMinX = 0;        // 카메라 좌상단 X 허용 최소
    int   m_scrollMaxX = 0;        // 카메라 좌상단 X 허용 최대

    // --- 셰이크 ---
    float m_shakeTime = 0.f;
    float m_shakeDuration = 0.15f;
    float m_shakeAmp = 8.f;
    POINT m_shakeOffset{ 0,0 };

    // --- 시네마틱 줌 ---
    float m_zoom = 1.0f;     // 1.0 = 기본, >1.0 줌-인
    POINT m_zoomFocus{ 0,0 };    // 월드 기준 포커스(보스 등)
    // 줌 보간용 타임라인
    bool  m_cineActive = false;
    float m_cineTime = 0.f;
    float m_cineDur = 0.f;
    float m_zoomStart = 1.0f;
    float m_zoomEnd = 1.0f;

    // --- 레터박스(상/하 블랙바, 화면좌표) ---
    int   m_barH = 0;        // 현재 바 높이(px)
    int   m_barTargetH = 0;        // 목표 바 높이(px)
    float m_barSpeed = 600.f;    // 등속(px/s)

    // --- 페이드(전체 화면 검은 오버레이, 0~255) ---
    float m_fadeTime = 0.f;
    float m_fadeDur = 0.f;
    int   m_fadeFrom = 0;
    int   m_fadeTo = 0;
    int   m_fadeAlpha = 0;

    // --- GDI 리소스 ---
    HDC     m_frontHDC = nullptr;
    HDC     m_backHDC = nullptr;
    HBITMAP m_hFrontBitmap = nullptr;   // 사용하지 않아도 안전을 위해 유지
    HBITMAP m_hBackBitmap = nullptr;
    HBITMAP m_hOldFrontBitmap = nullptr;
    HBITMAP m_hOldBackBitmap = nullptr;

    // 페이드용 검정 1x1 캐시
    HDC     m_fadeMemDC = nullptr;
    HBITMAP m_fadeBmp = nullptr;
    HBITMAP m_fadeOld = nullptr;

    // --- 내부 유틸 ---
    static float SmoothDamp1D(float current, float target, float& currentVelocity, float smoothTime, float dt)
    {
        if (smoothTime < 0.0001f) smoothTime = 0.0001f;
        const float omega = 2.0f / smoothTime;
        const float x = current - target;
        const float e = std::exp(-omega * dt);
        const float temp = (currentVelocity + omega * x) * dt;
        const float newVal = target + (x + temp) * e;
        currentVelocity = (currentVelocity - omega * temp) * e;
        return newVal;
    }

    CScreenManager() = default;
    ~CScreenManager() { Release(); }

    void Release()
    {
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
        if (m_fadeMemDC) {
            SelectObject(m_fadeMemDC, m_fadeOld);
            DeleteObject(m_fadeBmp);
            DeleteDC(m_fadeMemDC);
            m_fadeMemDC = nullptr; m_fadeBmp = nullptr; m_fadeOld = nullptr;
        }
    }

    void ensureFadeCache()
    {
        if (!m_fadeMemDC && m_frontHDC) {
            m_fadeMemDC = CreateCompatibleDC(m_frontHDC);
            m_fadeBmp = CreateCompatibleBitmap(m_frontHDC, 1, 1);
            m_fadeOld = (HBITMAP)SelectObject(m_fadeMemDC, m_fadeBmp);
            HBRUSH b = (HBRUSH)GetStockObject(BLACK_BRUSH);
            RECT r{ 0,0,1,1 };
            FillRect(m_fadeMemDC, &r, b); // 1x1 검정
        }
    }

public:
    CScreenManager(const CScreenManager&) = delete;
    CScreenManager& operator=(const CScreenManager&) = delete;

    static CScreenManager& Instance() {
        static CScreenManager inst;
        return inst;
    }

    // --- 초기화 ---
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

        // 초기 값
        m_posCamera = { 0,0 };
        m_targetCamera = { 0,0 };
        m_camRect = { 0,0, WINCX, WINCY };
        m_velX = m_velY = 0.f;

        m_scrollMinX = 0;
        m_scrollMaxX = std::max(0, m_iStageSize - WINCX);

        m_zoom = 1.0f;
        m_barH = m_barTargetH = 0;
        m_fadeAlpha = 0;

        ensureFadeCache();
    }

    // --- 그리기용 클리어(backHDC에) ---
    void Clear()
    {
        PatBlt(m_backHDC, m_posCamera.x, m_posCamera.y, ScreenCX, ScreenCY, WHITENESS);
    }

    // --- per-frame 업데이트: 셰이크, 스무딩, 시네마틱(줌/바), 페이드 ---
    void Update(double dt)
    {
        // 셰이크
        if (m_shakeTime > 0.f) {
            m_shakeTime -= (float)dt;
            if (m_shakeTime < 0.f) m_shakeTime = 0.f;
            const float k = (m_shakeTime / m_shakeDuration);
            const int ox = (rand() % 3 - 1) * (int)(m_shakeAmp * k);
            const int oy = (rand() % 3 - 1) * (int)(m_shakeAmp * k * 0.6f);
            m_shakeOffset = { ox, oy };
        }
        else {
            m_shakeOffset = { 0,0 };
        }

        // 스무스 추적 (m_posCamera -> m_targetCamera)
        float cx = (float)m_posCamera.x;
        float cy = (float)m_posCamera.y;
        float tx = (float)m_targetCamera.x;
        float ty = (float)m_targetCamera.y;

        cx = SmoothDamp1D(cx, tx, m_velX, m_smoothTime, (float)dt);
        cy = SmoothDamp1D(cy, ty, m_velY, m_smoothTime, (float)dt);

        m_posCamera.x = std::clamp((int)lroundf(cx), m_scrollMinX, m_scrollMaxX);
        m_posCamera.y = std::clamp((int)lroundf(cy), 0, MapSize * WINCY);
        SetCameraRect();

        // --- Cinematic: 줌 보간 + 레터박스 등속 ---
        if (m_cineDur > 0.f) {
            m_cineTime = std::min(m_cineTime + (float)dt, m_cineDur);
            float t = m_cineTime / m_cineDur;
            float s = t * t * (3.f - 2.f * t); // smoothstep
            m_zoom = m_zoomStart + (m_zoomEnd - m_zoomStart) * s;
        }
        if (m_barH != m_barTargetH) {
            int dir = (m_barTargetH > m_barH) ? 1 : -1;
            int delta = (int)std::round(m_barSpeed * dt) * dir;
            if ((dir > 0 && m_barH + delta > m_barTargetH) ||
                (dir < 0 && m_barH + delta < m_barTargetH))
                m_barH = m_barTargetH;
            else
                m_barH += delta;
        }

        // --- Fade ---
        if (m_fadeDur > 0.f && m_fadeAlpha != m_fadeTo) {
            m_fadeTime = std::min(m_fadeTime + (float)dt, m_fadeDur);
            float t = m_fadeTime / m_fadeDur;
            float s = t * t * (3.f - 2.f * t); // smoothstep
            m_fadeAlpha = (int)std::round(m_fadeFrom + (m_fadeTo - m_fadeFrom) * s);
            if (m_fadeTime >= m_fadeDur) {
                m_fadeAlpha = m_fadeTo;
                // 유지(필요시 m_fadeDur=0으로 타이머 종료)
                // m_fadeDur = 0.f;
            }
        }
    }

    // --- 최종 출력(back -> front, 줌/셰이크/레터박스/페이드 적용) ---
    void Present()
    {
        ensureFadeCache();

        HDC front = m_frontHDC;
        HDC back = m_backHDC;

        const float z = std::max(0.5f, m_zoom);

        if (fabsf(z - 1.0f) < 1e-3f) {
            // 일반(줌 없음): 기존처럼 BitBlt (카메라 + 셰이크)
            POINT src{ m_posCamera.x + m_shakeOffset.x,
                       m_posCamera.y + m_shakeOffset.y };
            src.x = std::max(0L, std::min(src.x, (LONG)(MapSize * ScreenCX - ScreenCX)));
            src.y = std::max(0L, std::min(src.y, (LONG)(MapSize * ScreenCY - ScreenCY)));
            BitBlt(front, 0, 0, ScreenCX, ScreenCY, back, src.x, src.y, SRCCOPY);
        }
        else {
            // --- 줌-인: back의 작은 사각(src) → WINCXxWINCY로 확대
            int srcW = (int)std::round(WINCX / z);
            int srcH = (int)std::round(WINCY / z);

            // 백버퍼 전체 크기(그려둔 월드용 backBitmap 기준)
            const int backW = MapSize * ScreenCX;
            const int backH = MapSize * ScreenCY;

            // 1) 포커스를 안전범위로 먼저 클램프 (경계에서도 중앙 유지)
            int safeFocusX = std::clamp((int)(m_zoomFocus.x + m_shakeOffset.x), srcW / 2, backW - srcW / 2);
            int safeFocusY = std::clamp((int)(m_zoomFocus.y + m_shakeOffset.y), srcH / 2, backH - srcH / 2);

            // 2) 안전 포커스 기준으로 srcRect 계산
            int srcX = safeFocusX - srcW / 2;
            int srcY = safeFocusY - srcH / 2;

            // (보호용) 혹시 모를 오차 클램프
            srcX = std::clamp(srcX, 0, std::max(0, backW - srcW));
            srcY = std::clamp(srcY, 0, std::max(0, backH - srcH));

            SetStretchBltMode(front, COLORONCOLOR);
            StretchBlt(front, 0, 0, WINCX, WINCY, back, srcX, srcY, srcW, srcH, SRCCOPY);
        }

        // --- 레터박스(상/하 블랙바), front에 직접 ---
        if (m_barH > 0) {
            HBRUSH black = (HBRUSH)GetStockObject(BLACK_BRUSH);
            RECT top{ 0,0, WINCX, m_barH };
            RECT bot{ 0, WINCY - m_barH, WINCX, WINCY };
            FillRect(front, &top, black);
            FillRect(front, &bot, black);
        }

        // --- 페이드(반투명 검정 오버레이), front에 직접 ---
        if (m_fadeAlpha > 0 && m_fadeMemDC) {
            BLENDFUNCTION bf{};
            bf.BlendOp = AC_SRC_OVER;
            bf.BlendFlags = 0;
            bf.SourceConstantAlpha = (BYTE)m_fadeAlpha; // 0~255
            bf.AlphaFormat = 0; // per-pixel alpha 없음

            // 1x1 검정을 화면 크기로 스트레치 알파 블렌드
            AlphaBlend(front, 0, 0, WINCX, WINCY, m_fadeMemDC, 0, 0, 1, 1, bf);
        }
    }

    // --- 접근자 ---
    HDC   GetHDC()       const { return m_backHDC; }
    HDC   GetFrontHDC()  const { return m_frontHDC; }
    RECT  GetCamRect()   const { return m_camRect; }
    POINT GetCamerPos()  const { return m_posCamera; }     // 흔들림 제외(논리 카메라)
    POINT GetShakeOffset() const { return m_shakeOffset; }

    void  SetCameraRect()
    {
        m_camRect = { m_posCamera.x, m_posCamera.y, m_posCamera.x + ScreenCX, m_posCamera.y + ScreenCY };
    }

    bool Get_ActiveZoom() { return m_cineActive; }

    // --- 스테이지 폭 설정 + 경계 복구 ---
    void  Set_StageSize(int stageWidth)
    {
        m_iStageSize = stageWidth;
        m_scrollMinX = 0;
        m_scrollMaxX = std::max(0, m_iStageSize - WINCX);
        m_targetCamera.x = std::clamp((int)m_targetCamera.x, m_scrollMinX, m_scrollMaxX);
        m_posCamera.x = std::clamp((int)m_posCamera.x, m_scrollMinX, m_scrollMaxX);
    }

    // --- 카메라 목표(부드럽게 이동) ---
    void  SetCameraTarget(POINT p, bool clampMap = true)
    {
        if (clampMap) {
            p.x = std::clamp((int)p.x, m_scrollMinX, m_scrollMaxX);
            p.y = std::clamp((int)p.y, 0, MapSize * WINCY);
        }
        m_targetCamera = p;
    }

    // --- 즉시 스냅 이동(컷신 등) ---
    void  SetCameraPos(POINT p, bool clampMap = true)
    {
        if (clampMap) {
            p.x = std::clamp((int)p.x, m_scrollMinX, m_scrollMaxX);
            p.y = std::clamp((int)p.y, 0, MapSize * WINCY);
        }
        m_posCamera = p;
        m_targetCamera = p;
        m_velX = m_velY = 0.f;
        SetCameraRect();
    }

    // --- 스무딩 강도 ---
    void  SetSmoothTime(float sec) { m_smoothTime = std::max(0.01f, sec); }

    // --- 카메라 X 경계 설정(보스 아레나 잠금 등) ---
    void  SetCameraBounds(int leftX, int rightX)
    {
        if (leftX > rightX) std::swap(leftX, rightX);
        m_scrollMinX = leftX;
        m_scrollMaxX = rightX;
        m_targetCamera.x = std::clamp((int)m_targetCamera.x, m_scrollMinX, m_scrollMaxX);
        m_posCamera.x = std::clamp((int)m_posCamera.x, m_scrollMinX, m_scrollMaxX);
    }
    void  ClearCameraBounds()
    {
        m_scrollMinX = 0;
        m_scrollMaxX = std::max(0, m_iStageSize - WINCX);
        m_targetCamera.x = std::clamp((int)m_targetCamera.x, m_scrollMinX, m_scrollMaxX);
        m_posCamera.x = std::clamp((int)m_posCamera.x, m_scrollMinX, m_scrollMaxX);
    }

    // --- 셰이크 트리거 ---
    void  CamShake(float amp = 8.f, float duration = 0.15f)
    {
        m_shakeAmp = amp;
        m_shakeDuration = duration;
        m_shakeTime = duration;
    }

    // --- 줌/레터박스/페이드 컨트롤 ---
    void  SetZoom(float z) { m_zoom = std::max(0.5f, z); }
    void  SetZoomFocus(POINT worldPt) { m_zoomFocus = worldPt; }
    void  SetLetterboxTarget(int targetH, float speed = 600.f)
    {
        m_barTargetH = std::max(0, targetH); m_barSpeed = speed;
    }

    // 시네마틱 시작(줌 보간 + 바 상승)
    void  StartCinematic(POINT focusWorld, float zoomTo, int barH, float durationSec)
    {
        m_cineActive = true;
        m_zoomFocus = focusWorld;
        m_zoomStart = m_zoom;
        m_zoomEnd = std::max(0.5f, zoomTo);
        m_cineTime = 0.f;
        m_cineDur = std::max(0.01f, durationSec);
        m_barTargetH = std::max(0, barH);
    }

    // 시네마틱 종료(줌 복귀 + 바 하강)
    void  StopCinematic()
    {
        m_cineActive = false;
        m_barTargetH = 0;
        m_zoomStart = m_zoom;
        m_zoomEnd = 1.0f;
        m_cineTime = 0.f;
        m_cineDur = 0.35f; // 복귀 속도(필요시 조절)
    }

    // 페이드
    void  StartFade(int from, int to, float durationSec)
    {
        m_fadeFrom = std::clamp(from, 0, 255);
        m_fadeTo = std::clamp(to, 0, 255);
        m_fadeDur = std::max(0.01f, durationSec);
        m_fadeTime = 0.f;
        m_fadeAlpha = m_fadeFrom;
    }
    void  FadeIn(float durationSec) { StartFade(255, 0, durationSec); }
    void  FadeOut(float durationSec) { StartFade(0, 255, durationSec); }
};
